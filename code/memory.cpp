struct MemBlock
{
	memsize size;
	memsize used;
	void* base;
};
internal void
init_memblock(MemBlock* block, memsize size, void* base)
{
	block->size = size;
	block->base = base;
	block->used = 0;
}

internal void*
alloc_bytes(MemBlock* block, memsize size)
{
	ASSERT((block->used + size) <= block->size);
	u8* base = (u8*)block->base;
	void* address = (void*)(base + block->used);
	block->used += size;
	return address;
}
#define alloc_struct(block, type) (type*)alloc_bytes(block, sizeof(type))
#define alloc_array(block, type, count) (type*)alloc_bytes(block, (count) * sizeof(type))

internal void
zero_bytes(void* base, memsize size)
{
	u8* mem = (u8*)base;
	for(memsize i = 0; i < size; ++i)
	{
		*mem++ = 0;
	}
}
#define zero_struct(s, type) zero_bytes(s, sizeof(type))
#define zero_array(a, type, count) zero_bytes(a, sizeof(type) * count)

internal void
mem_copy(void* src, void* dst, memsize size)
{
	u8* sp = (u8*)src;
	u8* dp = (u8*)dst;
	for(memsize i = 0; i < size; ++i)
	{
		*dp++ = *sp++;
	}
}