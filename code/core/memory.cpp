namespace memory
{
	enum BlockType
	{
		HEAP = 0,
		POOL = 1,
	};

	struct HeapTag
	{
		b32 in_use;
		memsize size;
	};

	struct Block
	{
		BlockType type;
		memsize size;
		memsize used;
		u8 alignment;
		void* base;
		u8* current;
	};

	fn void
	copy(void* src, void* dst, memsize size)
	{
		u8* sp = (u8*)src;
		u8* dp = (u8*)dst;
		for(memsize i = 0; i < size; ++i)
		{
			*dp++ = *sp++;
		}
	}

	fn void
	clear(void* base, memsize size)
	{
		u8* mem = (u8*)base;
		for(memsize i = 0; i < size; ++i)
		{
			*mem++ = 0;
		}
	}

	fn void
	set_block(Block* block, void* base, memsize size, BlockType type, u8 alignment)
	{
		block->size = size;
		block->base = base;
		block->used = 0;
		block->current = (u8*)base;
		block->type = type;
		block->alignment = alignment;

		if(type == BlockType::HEAP)
		{
			HeapTag tag = { false, size };
			auto tag_loc = (HeapTag*)block->current;
			*tag_loc = tag;
			block->used += sizeof(HeapTag);
		}
	}

	fn memsize
	get_padding(u8 alignment, memsize size)
	{
		return alignment - (size % alignment);
	}

	fn b32 
	check_available_memory(Block* block, memsize size)
	{
		return (block->used + size) < block->size;
	}

	fn void
	log_memory_usage(Block* block)
	{
		switch(block->type)
		{
			case memory::BlockType::POOL:
			{
				printf("Pool: %zu of %zu bytes used\n", block->used, block->size);
				break;
			}
			case memory::BlockType::HEAP:
			{
				printf("Heap: %zu of %zu bytes used\n", block->used, block->size);
				break;
			}
		}
	}

	fn void*
	pool_alloc(Block* block, memsize size)
	{
		auto padding = get_padding(block->alignment, size);
		size += padding;

		#if DEBUG
		if(check_available_memory(block, size) == false)
		{
			FAIL("Tried to allocate %zu bytes but only %zu bytes available in block\n", size, block->size - block->used);
		}
		#endif

		auto address = (void*)block->current;
		block->current += size;
		block->used += size;

		#if DEBUG
		log_memory_usage(block);
		#endif

		ASSERT(address, "Allocation returned null\n");
		return address;
	}

	fn void
	pool_reset(Block* block, b32 zero_out)
	{
		if(zero_out)
		{
			clear(block->base, block->size);
		}
		block->current = (u8*)block->base;
		block->used = 0;
	}

	fn void*
	heap_alloc(Block* block, memsize size)
	{
		auto padding = get_padding(block->alignment, size);

		//check if enough space first
		#if DEBUG
		if(check_available_memory(block, size) == false)
		{
			FAIL("Tried to allocate %zu bytes but only %zu bytes available in block\n", size, block->size - block->used);
		}
		#endif

		auto tag = (HeapTag*)block->current;

		int state = 0;
		for(;;)
		{
			b32 looped = false;
			switch(state)
			{
				case 0: // check tag
				{
					if(tag->in_use || size > tag->size)
					{
						state = 1;
						break;
					}

					// use 
					auto prev_size = tag->size;
					auto delta = prev_size - size;

					tag->in_use = true;
					tag->size = size;

					auto address = (void*)(block->current + sizeof(HeapTag));
					block->used += size;
					block->current += sizeof(HeapTag) + size;

					// Is there room for anything useful afterwards set a tag for it
					if(delta > sizeof(HeapTag) + 8)
					{
						auto next_tag = (HeapTag*)block->current;
						next_tag->in_use = false;
						next_tag->size = delta;
						block->used += sizeof(HeapTag);
					}
					//otherwise jump to the next tag
					else
					{
						//check to see if the jump has put us out of bounds of the block
						auto end = (u8*)block->current + block->size;
						if(block->current > end)
							block->current = (u8*)block->base;
					}

					#if DEBUG
					log_memory_usage(block);
					#endif

					return address;
				}
				case 1: // Find tag
				{
					block->current += tag->size + sizeof(HeapTag);
					auto end = (u8*)block->base + block->size;
					if(block->current > end)
					{
						if(looped)
						{
							FAIL("No free space available in heap memory");
						}
						block->current = (u8*)block->base;
						looped = true;
					}
					tag = (HeapTag*)block->current;
					state = 0;
					break;
				}
			}
		}
	}

	fn void
	heap_free(Block* block, void* location, memsize size)
	{
		#if DEBUG
		auto start = (u8*)block->base;
		auto end = start + block->size;
		auto pos = (u8*)location;
		if(pos < start || pos > end) FAIL("Memory location not part of this heap");
		#endif

		auto tag_location = (u8*)location - sizeof(HeapTag);
		auto tag = (HeapTag*)(tag_location);
		tag->in_use = false;
	}

	fn void
	heap_reset(Block* block, b32 zero_out)
	{
		if(zero_out) 
		{
			clear(block->base, block->size);
		}
		block->current = (u8*)block->base;
		auto tag = (HeapTag*)block->current;
		tag->in_use = false;
		tag->size = block->size;
		block->used = 0;
	}

	fn void* 
	alloc(Block* block, memsize size)
	{
		switch(block->type)
		{
			case BlockType::HEAP:
			{
				return heap_alloc(block, size);
			}
			case BlockType::POOL:
			{
				return pool_alloc(block, size);
			}
			default:
			{
				FAIL("Invalid block type");
			}	
		}
		return 0;
	}

	fn void
	dealloc(Block* block, void* location, memsize size)
	{
		switch(block->type)
		{
			case BlockType::HEAP:
			{
				return heap_free(block, location, size);
			}
			case BlockType::POOL:
			{
				FAIL("Can't dealloc items from a pool");
			}
			default:
			{
				FAIL("Invalid block type");
			}	
		}
	}

	fn void
	reset(Block* block, b32 zero_out)
	{
		switch(block->type)
		{
			case BlockType::HEAP:
			{
				heap_reset(block, zero_out);
				return;
			}
			case BlockType::POOL:
			{
				pool_reset(block, zero_out);
				return;
			}
			default:
			{
				FAIL("Invalid block type");
			}	
		}
	}
}

#define alloc_struct(block, type) (type*)memory::alloc(block, sizeof(type))
#define alloc_array(block, type, count) (type*)memory::alloc(block, (count) * sizeof(type))

#define clear_struct(block, s, type) memory::dealloc(block, s, sizeof(type))
#define clear_array(block, a, type, count) memory::dealloc(block, a, sizeof(type) * count)