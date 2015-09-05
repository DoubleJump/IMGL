struct Time
{
	f32 dt;
	f32 elapsed;
	f32 scale;
};

struct Platform
{
	memory::Block* storage; 
	input::Devices* devices; 
	renderer::State* render_state;
	Time time;
	b32 quit;
};

struct File
{
	u32 size;
	u8* offset;
	void* contents;
};

namespace file
{
	fn i32
	read_i32(File* f);

	fn string
	read_string(File* f);

	fn File 
	open(char* file_name);

	fn b32 
	write(char* file_name, File* f);

	fn void 
	close(File* f);
}