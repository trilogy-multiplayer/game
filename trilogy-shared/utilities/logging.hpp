#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <codecvt>
#include <vector>
#include <memory>
#include <windows.h>

#include <definitions.hpp>

class c_log
{
public:
	enum class color_t : std::uint32_t
	{
		RESET,
		BLACK, LBLACK,
		RED, LRED,
		GREEN, LGREEN,
		BLUE, LBLUE,
		YELLOW, LYELLOW,
		MAGENTA, LMAGENTA,
		CYAN, LCYAN,
		WHITE, LWHITE
	};

	using stdstream_manip = std::ostream& (*)(std::ostream&);
	using c_log_manip = c_log & (*)(c_log&);

	template<class T>
	c_log& put(const T& val)
	{
		if (TRILOGY_BUILD_CHANNEL == e_build_channel::DEVELOPMENT)
			std::cout << val;

		return *this;
	}

	c_log& put(bool val)
	{
		return put(val ? "true" : "false");
	}

	c_log& put(c_log_manip val)
	{
		return val(*this);
	}

	c_log& put(stdstream_manip val)
	{
		if (TRILOGY_BUILD_CHANNEL == e_build_channel::DEVELOPMENT)
			std::cout << val;

		return *this;
	}

	template<class... Args>
	c_log& put(c_log_manip val, const Args&... args)
	{
		return put(val).put(args...);
	}

	template<class T, class... Args>
	c_log& put(const T& val, const Args&... args)
	{
		return put(val).put(" ").put(args...);
	}

	c_log& put_time()
	{
		const time_t t = time(nullptr);
		tm tt;

		localtime_s(&tt, &t);

		std::cout << std::put_time(&tt, "[%H:%M:%S]") << std::flush;
		return *this;
	}

	c_log& put_color(color_t col)
	{
		*this << std::flush;

		for (auto& s : streams)
			s->put_color(col);

		return *this;
	}

	template<typename ...Args>
	static std::string Join(Args&& ...args)
	{
		std::ostringstream oss;
		(oss << ... << args);
		return oss.str();
	}


	template<class T> c_log& operator<<(const T& val) { return put(val); }
	c_log& operator<<(stdstream_manip val) { return put(val); }
	c_log& operator<<(c_log_manip val) { return put(val); }

	// Manipulators
	static c_log& Reset(c_log& log) { return log.put_color(color_t::RESET); }
	static c_log& Black(c_log& log) { return log.put_color(color_t::BLACK); }
	static c_log& LBlack(c_log& log) { return log.put_color(color_t::LBLACK); }
	static c_log& Red(c_log& log) { return log.put_color(color_t::RED); }
	static c_log& LRed(c_log& log) { return log.put_color(color_t::LRED); }
	static c_log& Green(c_log& log) { return log.put_color(color_t::GREEN); }
	static c_log& LGreen(c_log& log) { return log.put_color(color_t::LGREEN); }
	static c_log& Blue(c_log& log) { return log.put_color(color_t::BLUE); }
	static c_log& LBlue(c_log& log) { return log.put_color(color_t::LBLUE); }
	static c_log& Yellow(c_log& log) { return log.put_color(color_t::YELLOW); }
	static c_log& LYellow(c_log& log) { return log.put_color(color_t::LYELLOW); }
	static c_log& Magenta(c_log& log) { return log.put_color(color_t::MAGENTA); }
	static c_log& LMagenta(c_log& log) { return log.put_color(color_t::LMAGENTA); }
	static c_log& Cyan(c_log& log) { return log.put_color(color_t::CYAN); }
	static c_log& LCyan(c_log& log) { return log.put_color(color_t::LCYAN); }
	static c_log& White(c_log& log) { return log.put_color(color_t::WHITE); }
	static c_log& LWhite(c_log& log) { return log.put_color(color_t::LWHITE); }
	static c_log& Endl(c_log& log) { return log.put(std::endl).put(std::dec).put_color(color_t::WHITE); }
	static c_log& Time(c_log& log) { return log.put_time(); }

	struct c_log_base
	{
		virtual c_log& begin() const = 0;
		virtual bool force_new_line() const { return true; };
		template<class... Args> c_log& operator()(const Args&... args) const {
			c_log& return_value = begin().put<Args...>(args...);

			if (force_new_line())

				return_value.put(Endl);
			return return_value;
		}
		template<class T> c_log& operator<<(const T& val) const { return begin().put<T>(val); }
	};

	static constexpr struct c_log_raw : public c_log_base {
		c_log& begin() const override { return instance(); }
	} Raw{};

	static constexpr struct c_log_Error : public c_log_base {
		c_log& begin() const override { return instance().put(LYellow, LRed, " - [ERROR]", Reset); }
	} Error{};

	static constexpr struct c_log_debug : public c_log_base {
		c_log& begin() const override { return instance().put(LYellow, Cyan, " - [DEBUG]", Reset); }
	} Debug{};

	static constexpr struct c_log_error : public c_log_base {
		c_log& begin() const override { return instance().put(LYellow, LBlue, " - [TRILOGY]", Reset); }
	} Trilogy{};

	static constexpr struct c_log_info : public c_log_base {
	private:
		c_log& begin() const override { return instance().put(LYellow, Yellow, " - [INFO]", Reset); }
	} Info{};

	static constexpr struct c_log_input : public c_log_base {
	private:
		c_log& begin() const override { return instance().put(LYellow, Green, " - [INPUT]", Reset); }
		bool force_new_line() const override { return false; }
	} Input{};

	static c_log& instance() noexcept
	{
		static c_log s;
		return s;
	}

	class c_stream
	{
	public:
		virtual c_stream& put(const std::string& val) = 0;
		virtual c_stream& put_color(color_t color) = 0;
	};

	static void add_out(c_stream* stream) { instance().streams.emplace_back(stream); }

	class c_log_consolestream : public c_stream
	{
	public:
		c_stream& put(const std::string& val) override
		{
			std::cout << val;

			return *this;
		}

		c_stream& put_color(color_t val) override
		{
			WORD col = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;

			switch (val)
			{
			case color_t::BLACK:
				col = 0;
				break;
			case color_t::LBLACK:
				col = FOREGROUND_INTENSITY;
				break;
			case color_t::RED:
				col = FOREGROUND_RED;
				break;
			case color_t::LRED:
				col = FOREGROUND_RED | FOREGROUND_INTENSITY;
				break;
			case color_t::GREEN:
				col = FOREGROUND_GREEN;
				break;
			case color_t::LGREEN:
				col = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
				break;
			case color_t::BLUE:
				col = FOREGROUND_BLUE;
				break;
			case color_t::LBLUE:
				col = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
				break;
			case color_t::YELLOW:
				col = FOREGROUND_RED | FOREGROUND_GREEN;
				break;
			case color_t::LYELLOW:
				col = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
				break;
			case color_t::MAGENTA:
				col = FOREGROUND_RED | FOREGROUND_BLUE;
				break;
			case color_t::LMAGENTA:
				col = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
				break;
			case color_t::CYAN:
				col = FOREGROUND_GREEN | FOREGROUND_BLUE;
				break;
			case color_t::LCYAN:
				col = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
				break;
			case color_t::WHITE:
				col = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
				break;
			case color_t::LWHITE:
				col = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
				break;
			}

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), col);

			return *this;
		}

	};

	c_log() { };
	c_log(const c_log&) = delete;
	c_log& operator=(const c_log&) = delete;

	std::vector<std::unique_ptr<c_stream>> streams;
};