namespace file
{
	fn i32
	read_i32(File* f)
	{
		auto r = *((i32*)f->offset);
		f->offset += sizeof(i32);
		return r;
	}

	fn i32*
	read_i32_array(File* f)
	{
		auto length = read_i32(f);
		auto r = (i32*)f->offset;
		f->offset += sizeof(i32) * length;
		return r;
	}

	fn f32
	read_f32(File* f)
	{
		auto r = *((f32*)f->offset);
		f->offset += sizeof(f32);
		return r;
	}

	fn f32*
	read_f32_array(File* f)
	{
		auto length = read_i32(f);
		auto r = (f32*)f->offset;
		f->offset += sizeof(f32) * length;
		return r;
	}

	fn string
	read_string(File* f)
	{
		string r;
		r.length = read_i32(f);
		r.data = (char*)f->offset;
		f->offset += r.length;
		return r;
	}

	fn void
	set_offset(File* f, memsize offset)
	{
		f->offset = (u8*)f->contents + offset;
	}

	fn void
	close(File* f)
	{
		if(f->contents)
		{
			VirtualFree(f->contents, 0, MEM_RESERVE);
			f->contents = 0;
			f->size = 0;
			f->offset = 0;
		}
	}

	fn File 
	open(char* path)
	{
		File result = {};

		auto handle = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
		if(handle == INVALID_HANDLE_VALUE)
		{
			FAIL("Could not open file: %s\n", path);
		}

		LARGE_INTEGER file_size;
		auto success = GetFileSizeEx(handle, &file_size);
		if(!success)
		{
			FAIL("Could not get file size");
		}
			
		auto file_size32 = math::safe_truncate_u64(file_size.QuadPart); // Dont open files larger than 4GB
		result.contents = VirtualAlloc(0, file_size32, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
		if(!result.contents)
		{
			FAIL("Could not read file contents into memory");
		}

		DWORD bytes_read;
		success = ReadFile(handle, result.contents, file_size32, &bytes_read, 0) && (file_size32 == bytes_read);
		if(!success)
		{
			file::close(&result);
		}

		result.size = file_size32;
		result.offset = (u8*)result.contents;

		CloseHandle(handle);
		return result;
	}

	fn b32
	write(char* path, File* f)
	{
		auto handle = CreateFileA(path, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
		if(handle == INVALID_HANDLE_VALUE)
		{
			FAIL("Could not create file at path: %s \n", path);
			return false;
		}

		DWORD bytes_written;
		auto success = WriteFile(handle, f->contents, f->size, &bytes_written, 0);
		if(!success)
		{
			FAIL("Could not write memory to file");
			return false;
		}

		CloseHandle(handle);
		return (bytes_written == f->size);
	}
}
