namespace memory
{
	struct Block
	{
		memsize size;
		memsize used;
		void* base;
	};
	fn void
	set_block(Block& block, memsize size, void* base)
	{
		block.size = size;
		block.base = base;
		block.used = 0;
	}

	fn void*
	alloc(Block& block, memsize size)
	{
		ASSERT((block.used + size) <= block.size);
		u8* base = (u8*)block.base;
		void* address = (void*)(base + block.used);
		block.used += size;
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

#define alloc_struct(block, type) (type*)memory::alloc(block, sizeof(type))
#define alloc_array(block, type, count) (type*)memory::alloc(block, (count) * sizeof(type))

#define clear_struct(s, type) memory::clear(s, sizeof(type))
#define clear_array(a, type, count) memory::clear(a, sizeof(type) * count)