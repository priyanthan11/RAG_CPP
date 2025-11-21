#include "chunker.h"
#include <sstream>
#include <iomanip>
#include <functional>



DocumentChunker::DocumentChunker(int chunk_size, int overlap):
	chunk_size_(chunk_size),
	overlap_(overlap)
{
}

std::vector<Chunk> DocumentChunker::chunk_text(const std::string& text, const std::string& source)
{
	std::vector<Chunk> out;
	size_t len = text.size();

	if (len == 0) return out;
	size_t step = std::max<int>(1, chunk_size_ - overlap_);
	
	for (size_t start = 0; start < len; start += step)
	{
		size_t end = std::min(len, start + chunk_size_);
		Chunk c;

		c.id = make_id(source, start);
		c.text = text.substr(start, end - start);
		c.source = source;
		c.start_char = start;
		c.end_char = end;
		c.meta = { {"source",source},{"start",start},{"end",end} };
		out.push_back(std::move(c));
		if (end == len) break;
	}



	return out;
}

std::string DocumentChunker::make_id(const std::string& source, size_t start)
{
	std::ostringstream ss;
	ss << source << ":" << start;
	// simple stable hash
	std::hash<std::string> h;

	return std::to_string(h(ss.str()));
}
