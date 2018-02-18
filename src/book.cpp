// The MIT License (MIT)

// Copyright (c) 2017 nabijaczleweli

// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


#include "book.hpp"
#include "data.hpp"
#include "util.hpp"
#include <algorithm>
#include <cpr/cpr.h>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <memory>
#include <minizip/zip.h>
#include <set>
#include <sstream>


struct zipFile_deleter {
	template <class T>
	void operator()(T * f) {
		zipClose(f, "ePub book");
	}
};


book book::from(const include_order & incdirs, bool free_date, std::istream & descriptor) {
	detail::book_parser p{};
	p.incdirs   = &incdirs;
	p.free_date = free_date;

	for(std::string line; std::getline(descriptor, line);)
		p.take_line(line);

	book b{};
	p.construct(b);
	return b;
}

book book::from(const include_order & incdirs, bool free_date, const std::string & descriptor) {
	return book::from(incdirs, free_date, descriptor.c_str());
}

book book::from(const include_order & incdirs, bool free_date, const char * descriptor) {
	detail::book_parser p{};
	p.incdirs   = &incdirs;
	p.free_date = free_date;

	for(const char *line_s = descriptor, *line_e = nullptr; (line_e = std::strchr(line_s, '\n'));) {
		p.take_line(line_s, line_e - line_s);
		line_s = line_e + 1;
	}

	book b{};
	p.construct(b);
	return b;
}

bool operator==(const content_element & lhs, const content_element & rhs) {
	return lhs.id == rhs.id && lhs.filename == rhs.filename && lhs.data == rhs.data;
}

bool operator==(const content_data & lhs, const content_data & rhs) {
	return lhs.type == rhs.type && lhs.value == rhs.value;
}

bool operator==(const book & lhs, const book & rhs) {
	return lhs.content == rhs.content && lhs.non_content == rhs.non_content && lhs.name == rhs.name && lhs.cover == rhs.cover && lhs.author == rhs.author &&
	       lhs.date.first.tm_min == rhs.date.first.tm_min && lhs.date.first.tm_hour == rhs.date.first.tm_hour &&
	       lhs.date.first.tm_mday == rhs.date.first.tm_mday && lhs.date.first.tm_mon == rhs.date.first.tm_mon &&
	       lhs.date.first.tm_year == rhs.date.first.tm_year && lhs.date.first.tm_wday == rhs.date.first.tm_wday &&
	       lhs.date.first.tm_yday == rhs.date.first.tm_yday && lhs.date.first.tm_isdst == rhs.date.first.tm_isdst && lhs.date.second == rhs.date.second &&
	       lhs.language == rhs.language && lhs.description == rhs.description;
}

void book::write_to(const char * path) {
	std::unique_ptr<void, zipFile_deleter> epub(zipOpen(path, APPEND_STATUS_CREATE));

	zipOpenNewFileInZip(epub.get(), "META-INF/container.xml", nullptr, nullptr, 0, nullptr, 0, "container file", Z_DEFLATED, Z_BEST_COMPRESSION);
	zipWriteInFileInZip(epub.get(), assets_container_xml, std::strlen(assets_container_xml));
	zipCloseFileInZip(epub.get());

	zipOpenNewFileInZip(epub.get(), "mimetype", nullptr, nullptr, 0, nullptr, 0, "mimetype", Z_DEFLATED, Z_BEST_COMPRESSION);
	zipWriteInFileInZip(epub.get(), assets_mimetype, std::strlen(assets_mimetype));
	zipCloseFileInZip(epub.get());

	write_content_table(epub.get());
	write_table_of_contents(epub.get());

	std::vector<std::string> written;
	if(cover)
		write_element(epub.get(), *cover, written);
	std::for_each(content.begin(), content.end(), [&](auto && ctnt) { this->write_element(epub.get(), ctnt, written); });
	std::for_each(non_content.begin(), non_content.end(), [&](auto && nctnt) { this->write_element(epub.get(), nctnt, written); });
}

void book::write_content_table(void * epub) {
	zipOpenNewFileInZip(epub, "content.opf", nullptr, nullptr, 0, nullptr, 0, "content table", Z_DEFLATED, Z_BEST_COMPRESSION);
	zipWriteInFileInZip(epub, assets_content_opf_header, std::strlen(assets_content_opf_header));

	zipWriteInFileInZip(epub, "    <dc:title>", std::strlen("    <dc:title>"));
	zipWriteInFileInZip(epub, name.c_str(), name.size());
	zipWriteInFileInZip(epub, "</dc:title>\n", std::strlen("</dc:title>\n"));

	zipWriteInFileInZip(epub, "    <dc:creator opf:role=\"aut\">", std::strlen("    <dc:creator opf:role=\"aut\">"));
	zipWriteInFileInZip(epub, author.c_str(), author.size());
	zipWriteInFileInZip(epub, "</dc:creator>\n", std::strlen("</dc:creator>\n"));

	auto id_s = id.string();
	zipWriteInFileInZip(epub, "    <dc:identifier id=\"uuid\" opf:scheme=\"uuid\">", std::strlen("    <dc:identifier id=\"uuid\" opf:scheme=\"uuid\">"));
	zipWriteInFileInZip(epub, id_s, std::strlen(id_s));
	zipWriteInFileInZip(epub, "</dc:identifier>\n", std::strlen("</dc:identifier>\n"));
	std::free(id_s);

	std::ostringstream date_s;
	date_s << std::put_time(&date.first, "%Y-%m-%dT%H:%M:%S");
	zipWriteInFileInZip(epub, "    <dc:date>", std::strlen("    <dc:date>"));
	zipWriteInFileInZip(epub, date_s.str().c_str(), date_s.str().size());
	zipWriteInFileInZip(epub, date.second.c_str(), date.second.size());
	zipWriteInFileInZip(epub, "</dc:date>\n", std::strlen("</dc:date>\n"));

	if(description) {
		zipWriteInFileInZip(epub, "    <dc:description>\n", std::strlen("    <dc:description>\n"));
		write_element_data(epub, *description, false);
		zipWriteInFileInZip(epub, "    </dc:description>\n", std::strlen("    </dc:description>\n"));
	}

	if(cover) {
		zipWriteInFileInZip(epub, "    <meta name=\"cover\" content=\"", std::strlen("    <meta name=\"cover\" content=\""));
		zipWriteInFileInZip(epub, cover->id.c_str(), cover->id.size());
		zipWriteInFileInZip(epub, "\" />\n", std::strlen("\" />\n"));
	}

	zipWriteInFileInZip(epub, "  </metadata>\n", std::strlen("  </metadata>\n"));
	zipWriteInFileInZip(epub, "  <manifest>\n", std::strlen("  <manifest>\n"));
	zipWriteInFileInZip(epub, assets_content_opf_manifest_toc_line, std::strlen(assets_content_opf_manifest_toc_line));

	std::set<std::string> specified_ids;
	auto spec_item = [&, specified_ids = std::set<std::string>{} ](auto && elem) mutable {
		if(specified_ids.find(elem.id) != specified_ids.end())
			return;
		specified_ids.emplace(elem.id);

		const auto dot_id = elem.filename.find_last_of('.');
		auto mime_itr     = mime_types.find("text");
		if(dot_id != std::string::npos && dot_id != elem.filename.size() - 1) {
			mime_itr = mime_types.find(elem.filename.c_str() + dot_id + 1);
			if(mime_itr == mime_types.end())
				throw "No MIME type for extension " + std::string(elem.filename.c_str() + dot_id + 1) + ".";
		}

		zipWriteInFileInZip(epub, "    <item href=\"", std::strlen("    <item href=\""));
		zipWriteInFileInZip(epub, elem.filename.c_str(), elem.filename.size());
		zipWriteInFileInZip(epub, "\" id=\"", std::strlen("\" id=\""));
		zipWriteInFileInZip(epub, elem.id.c_str(), elem.id.size());
		zipWriteInFileInZip(epub, "\" media-type=\"", std::strlen("\" media-type=\""));
		zipWriteInFileInZip(epub, mime_itr->second, std::strlen(mime_itr->second));
		zipWriteInFileInZip(epub, "\" />\n", std::strlen("\" />\n"));
	};
	if(cover)
		spec_item(*cover);
	std::for_each(content.begin(), content.end(), spec_item);
	std::for_each(non_content.begin(), non_content.end(), spec_item);

	zipWriteInFileInZip(epub, "  </manifest>\n", std::strlen("  </manifest>\n"));
	zipWriteInFileInZip(epub, "  <spine toc=\"toc\">\n", std::strlen("  <spine toc=\"toc\">\n"));

	for(auto && ctnt : content) {
		zipWriteInFileInZip(epub, "    <itemref idref=\"", std::strlen("    <itemref idref=\""));
		zipWriteInFileInZip(epub, ctnt.id.c_str(), ctnt.id.size());
		zipWriteInFileInZip(epub, "\" />\n", std::strlen("\" />\n"));
	}

	zipWriteInFileInZip(epub, "  </spine>\n", std::strlen("  </spine>\n"));
	zipWriteInFileInZip(epub, "  <guide>\n", std::strlen("  <guide>\n"));

	if(cover) {
		zipWriteInFileInZip(epub, "    <reference xmlns=\"http://www.idpf.org/2007/opf\" href=\"",
		                    std::strlen("    <reference xmlns=\"http://www.idpf.org/2007/opf\" href=\""));
		zipWriteInFileInZip(epub, cover->filename.c_str(), cover->filename.size());
		zipWriteInFileInZip(epub, "\" title=\"", std::strlen("\" title=\""));
		zipWriteInFileInZip(epub, cover->id.c_str(), cover->id.size());
		zipWriteInFileInZip(epub, "\" type=\"cover\" />\n", std::strlen("\" type=\"cover\" />\n"));
	}

	zipWriteInFileInZip(epub, assets_content_opf_guide_toc_line, std::strlen(assets_content_opf_guide_toc_line));

	zipWriteInFileInZip(epub, "  </guide>\n", std::strlen("  </guide>\n"));
	zipWriteInFileInZip(epub, "</package>\n", std::strlen("</package>\n"));

	zipCloseFileInZip(epub);
}

void book::write_table_of_contents(void * epub) {
	zipOpenNewFileInZip(epub, "toc.ncx", nullptr, nullptr, 0, nullptr, 0, "table of contents", Z_DEFLATED, Z_BEST_COMPRESSION);
	zipWriteInFileInZip(epub, "<?xml version='1.0' encoding='utf-8'?>\n", std::strlen("<?xml version='1.0' encoding='utf-8'?>\n"));

	zipWriteInFileInZip(epub, "<ncx xmlns=\"http://www.daisy.org/z3986/2005/ncx/\" version=\"2005-1\" xml:lang=\"",
	                    std::strlen("<ncx xmlns=\"http://www.daisy.org/z3986/2005/ncx/\" version=\"2005-1\" xml:lang=\""));
	zipWriteInFileInZip(epub, language.c_str(), language.size());
	zipWriteInFileInZip(epub, "\">\n", std::strlen("\">\n"));

	zipWriteInFileInZip(epub, "  <head>\n", std::strlen("  <head>\n"));

	auto id_s = id.string();
	zipWriteInFileInZip(epub, "    <meta content=\"", std::strlen("    <meta content=\""));
	zipWriteInFileInZip(epub, id_s, std::strlen(id_s));
	zipWriteInFileInZip(epub, "\" name=\"dtb:uid\" />\n", std::strlen("\" name=\"dtb:uid\" />\n"));
	std::free(id_s);

	zipWriteInFileInZip(epub, "    <meta content=\"2\" name=\"dtb:depth\" />\n", std::strlen("    <meta content=\"2\" name=\"dtb:depth\" />\n"));
	zipWriteInFileInZip(epub, "  </head>\n", std::strlen("  </head>\n"));
	zipWriteInFileInZip(epub, "  <docTitle>\n", std::strlen("  <docTitle>\n"));

	zipWriteInFileInZip(epub, "    <text>", std::strlen("    <text>"));
	zipWriteInFileInZip(epub, name.c_str(), name.size());
	zipWriteInFileInZip(epub, "</text>\n", std::strlen("</text>\n"));

	zipWriteInFileInZip(epub, "  </docTitle>\n", std::strlen("  </docTitle>\n"));
	zipWriteInFileInZip(epub, "  <navMap>\n", std::strlen("  <navMap>\n"));

	auto nav_map_id = 0u;
	for(auto && ctnt : content) {
		if(ctnt.data.type != content_type::path)
			continue;

		const auto title = get_ebook_title(ctnt.data.value);
		if(!title)
			continue;

		++nav_map_id;

		zipWriteInFileInZip(epub, "    <navPoint id=\"", std::strlen("    <navPoint id=\""));
		zipWriteInFileInZip(epub, ctnt.id.c_str(), ctnt.id.size());
		zipWriteInFileInZip(epub, "\" playOrder=\"", std::strlen("\" playOrder=\""));
		zipWriteInFileInZip(epub, std::to_string(nav_map_id).c_str(), std::to_string(nav_map_id).size());
		zipWriteInFileInZip(epub, "\">\n", std::strlen("\">\n"));

		zipWriteInFileInZip(epub, "      <navLabel>\n", std::strlen("      <navLabel>\n"));

		zipWriteInFileInZip(epub, "        <text>", std::strlen("        <text>"));
		zipWriteInFileInZip(epub, title->c_str(), title->size());
		zipWriteInFileInZip(epub, "</text>\n", std::strlen("</text>\n"));

		zipWriteInFileInZip(epub, "      </navLabel>\n", std::strlen("      </navLabel>\n"));

		zipWriteInFileInZip(epub, "      <content src=\"", std::strlen("      <content src=\""));
		zipWriteInFileInZip(epub, ctnt.filename.c_str(), ctnt.filename.size());
		zipWriteInFileInZip(epub, "\" />\n", std::strlen("\" />\n"));

		zipWriteInFileInZip(epub, "    </navPoint>\n", std::strlen("    </navPoint>\n"));
	}

	zipWriteInFileInZip(epub, "  </navMap>\n", std::strlen("  </navMap>\n"));
	zipWriteInFileInZip(epub, "</ncx>\n", std::strlen("</ncx>\n"));

	zipCloseFileInZip(epub);
}

void book::write_element(void * epub, const content_element & elem, std::vector<std::string> & written) {
	if(std::find(written.begin(), written.end(), elem.filename) == written.end())
		written.emplace_back(elem.filename);
	else
		return;

	zipOpenNewFileInZip(epub, elem.filename.c_str(), nullptr, nullptr, 0, nullptr, 0, elem.id.c_str(), Z_DEFLATED, Z_BEST_COMPRESSION);
	write_element_data(epub, elem.data);
	zipCloseFileInZip(epub);
}

void book::write_element_data(void * epub, const content_data & data, bool wrap_strings) {
	switch(data.type) {
		case content_type::path: {
			char buf[1024];
			std::ifstream in_data(data.value, std::ios::binary);

			for(;;) {
				in_data.read(buf, sizeof(buf));
				const auto read = in_data.gcount();

				zipWriteInFileInZip(epub, buf, read);
				if(read != sizeof(buf))
					break;
			}
		} break;
		case content_type::string:
			if(wrap_strings)
				zipWriteInFileInZip(epub, assets_string_data_html_head, std::strlen(assets_string_data_html_head));
			zipWriteInFileInZip(epub, data.value.c_str(), data.value.size());
			if(wrap_strings)
				zipWriteInFileInZip(epub, assets_string_data_html_tail, std::strlen(assets_string_data_html_tail));
			break;
		case content_type::network: {
			const auto in_data = cpr::Get(cpr::Url(data.value)).text;
			zipWriteInFileInZip(epub, in_data.c_str(), in_data.size());
		} break;
	}
}
