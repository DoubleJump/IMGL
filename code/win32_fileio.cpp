internal DEBUG_FileResult 
DEBUG_read_file(ThreadContext* thread, char* file_name)
{
	DEBUG_FileResult result = {};

	auto file_handle = CreateFileA(file_name, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	if(file_handle == INVALID_HANDLE_VALUE)
	{
		ASSERT(!"Could not open file");
		return result;
	}

	LARGE_INTEGER file_size;
	auto success = GetFileSizeEx(file_handle, &file_size);
	if(!success)
	{
		ASSERT(!"Could not get file size");
		return result;
	}
		
	auto file_size32 = math::safe_truncate_u64(file_size.QuadPart); // Dont open files larger than 4GB
	result.contents = VirtualAlloc(0, file_size32, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if(!result.contents)
	{
		ASSERT(!"Could not read file contents into memory");
		return result;
	}

	DWORD bytes_read;
	success = ReadFile(file_handle, result.contents, file_size32, &bytes_read, 0) && (file_size32 == bytes_read);
	if(!success)
	{
		DEBUG_free_file(thread, result.contents);
		result.contents = 0;
		return result;
	}

	result.size = file_size32;

	CloseHandle(file_handle);
	return result;
}

internal b32
DEBUG_write_file(ThreadContext* thread, char* file_name, u32 size, void* memory)
{
	auto file_handle = CreateFileA(file_name, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
	if(file_handle == INVALID_HANDLE_VALUE)
	{
		ASSERT(!"Could not create file");
		return false;
	}

	DWORD bytes_written;
	auto success = WriteFile(file_handle, memory, size, &bytes_written, 0);
	if(!success)
	{
		ASSERT(!"Could not write memory to file");
		return false;
	}

	CloseHandle(file_handle);
	return (bytes_written == size);
}

internal void
DEBUG_free_file(ThreadContext* thread, void* memory)
{
	if(memory)
	{
		VirtualFree(memory, 0, MEM_RESERVE);
	}
}