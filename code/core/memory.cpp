namespace memory
{
	struct Block
	{
		memsize size;
		memsize used;
		void* base;
	};

	//Block* ctx;

	fn void
	set_block(Block* block, memsize size, void* base)
	{
		block->size = size;
		block->base = base;
		block->used = 0;
	}

	fn void*
	push_bytes(Block* block, memsize size)
	{
		#if BUILD_INTERNAL
			if((block->used + size) <= block->size)
			{
				printf("Tried to allocate %zu bytes but only %zu bytes available in block\n", size, block->size - block->used);
				FAIL;
			}
		#endif

		u8* base = (u8*)block->base;
		void* address = (void*)(base + block->used);
		block->used += size;

		#if BUILD_INTERNAL
			printf("Memory Used: %.1f/%.1f KB\n", (f32)block->used / 1024.0f, (f32)block->size / 1024.0f);
		#endif

		return address;
	}

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
}

#define push_struct(block, type) (type*)memory::push_bytes(block, sizeof(type))
#define push_array(block, type, count) (type*)memory::push_bytes(block, (count) * sizeof(type))

#define clear_struct(s, type) memory::clear(s, sizeof(type))
#define clear_array(a, type, count) memory::clear(a, sizeof(type) * count)