#include "game/state/gametime.h"
#include "library/strings_format.h"
#include <array>
#include <chrono>
#include <iomanip>
#include <locale>
#include <sstream>

using namespace std::chrono;
// using namespace date;

namespace OpenApoc
{

static const auto GAME_START = sys_days{year{2084} / March / 7};

static nanoseconds ticksToPosix(int64_t ticks)
{
	using tick_duration = duration<double>;
	return duration_cast<nanoseconds>(tick_duration{ticks} / TICKS_PER_SECOND);
}

GameTime::GameTime(uint64_t ticks) : ticks(ticks) {};

static sys_time<nanoseconds> getPtime(uint64_t ticks) { return GAME_START + ticksToPosix(ticks); }

UString GameTime::getLongTimeString() const
{
	auto timePoint = getPtime(this->ticks);
	auto timePointRounded = floor<seconds>(timePoint);
	return std::format("{:%T}", timePointRounded);
}

UString GameTime::getShortTimeString() const
{
	auto timePoint = getPtime(this->ticks);
	return std::format("{:%R}", timePoint);
}

UString GameTime::getLongDateString() const
{

	std::stringstream ss;
	auto timePoint = getPtime(this->ticks);
	auto currentDate = floor<days>(timePoint);
	auto ymd = year_month_day{currentDate};
	auto weekDay = weekday{ymd};

	static const std::array<std::string, 12> months = {
	    tr("January"),   tr("February"), tr("March"),    tr("April"),
	    tr("May"),       tr("June"),     tr("July"),     tr("August"),
	    tr("September"), tr("October"),  tr("November"), tr("December")};

	static const std::array<std::string, 7> weekdays = {
	    tr("Sunday"),   tr("Monday"), tr("Tuesday"), tr("Wednesday"),
	    tr("Thursday"), tr("Friday"), tr("Saturday")};

	static const std::array<std::string, 31> days = {
	    tr("1st"),  tr("2nd"),  tr("3rd"),  tr("4th"),  tr("5th"),  tr("6th"),  tr("7th"),
	    tr("8th"),  tr("9th"),  tr("10th"), tr("11th"), tr("12th"), tr("13th"), tr("14th"),
	    tr("15th"), tr("16th"), tr("17th"), tr("18th"), tr("19th"), tr("20th"), tr("21st"),
	    tr("22nd"), tr("23rd"), tr("24th"), tr("25th"), tr("26th"), tr("27th"), tr("28th"),
	    tr("29th"), tr("30th"), tr("31st")};

	ss << weekdays[weekDay.c_encoding()] << ", " << days[static_cast<unsigned>(ymd.day()) - 1]
	   << " " << months[static_cast<unsigned>(ymd.month()) - 1] << ", "
	   << static_cast<int>(ymd.year());

	return ss.str();
}

UString GameTime::getShortDateString() const
{
	std::stringstream ss;
	auto timePoint = getPtime(this->ticks);
	auto currentDate = floor<days>(timePoint);
	auto ymd = year_month_day{currentDate};

	static const std::array<std::string, 12> months = {
	    tr("January"),   tr("February"), tr("March"),    tr("April"),
	    tr("May"),       tr("June"),     tr("July"),     tr("August"),
	    tr("September"), tr("October"),  tr("November"), tr("December")};

	static const std::array<std::string, 31> days = {
	    tr("1st"),  tr("2nd"),  tr("3rd"),  tr("4th"),  tr("5th"),  tr("6th"),  tr("7th"),
	    tr("8th"),  tr("9th"),  tr("10th"), tr("11th"), tr("12th"), tr("13th"), tr("14th"),
	    tr("15th"), tr("16th"), tr("17th"), tr("18th"), tr("19th"), tr("20th"), tr("21st"),
	    tr("22nd"), tr("23rd"), tr("24th"), tr("25th"), tr("26th"), tr("27th"), tr("28th"),
	    tr("29th"), tr("30th"), tr("31st")};

	ss << days[static_cast<unsigned>(ymd.day()) - 1] << " "
	   << months[static_cast<unsigned>(ymd.month()) - 1] << ", " << static_cast<int>(ymd.year());

	return ss.str();
}

UString GameTime::getWeekString() const
{
	return UString(fmt::format("{} {}", tr("Week"), getWeek()));
}

unsigned int GameTime::getMonth() const
{
	auto currentDate = year_month_day{floor<days>(getPtime(this->ticks))};
	auto startDate = year_month_day{floor<days>(GAME_START)};

	const int months =
	    (static_cast<int>(currentDate.year()) - static_cast<int>(startDate.year())) * 12 +
	    static_cast<unsigned>(currentDate.month()) - static_cast<unsigned>(startDate.month());

	return months;
}

unsigned int GameTime::getWeek() const
{
	auto gameStartDate = floor<days>(GAME_START);
	auto currentDate = floor<days>(getPtime(this->ticks));

	auto gameStartWeekday = weekday{gameStartDate};
	auto firstMonday = gameStartDate - (gameStartWeekday - Monday);

	auto currentWeekday = weekday{currentDate};
	auto lastMonday = currentDate - (currentWeekday - Monday);

	auto daysDiff = (lastMonday - firstMonday).count();
	return daysDiff / 7 + 1;
}

unsigned int GameTime::getFirstDayOfCurrentWeek() const
{
	auto today = floor<days>(getPtime(this->ticks));
	auto todayWeekday = weekday{today};

	auto daysToMonday = (todayWeekday == Sunday) ? days{6} : (todayWeekday - Monday);

	auto firstDayOfWeek = today - daysToMonday;
	return static_cast<unsigned>(year_month_day{firstDayOfWeek}.day());
}

unsigned int GameTime::getLastDayOfCurrentWeek() const
{
	auto todayWeekday = weekday{floor<days>(getPtime(this->ticks))};

	unsigned int daysBeforeWeekEnd;
	if (todayWeekday == Sunday)
	{
		daysBeforeWeekEnd = 0;
	}
	else
	{
		daysBeforeWeekEnd = (Sunday - todayWeekday).count();
	}

	auto endOfWeekTime = getPtime(this->ticks + (daysBeforeWeekEnd * TICKS_PER_DAY));
	auto endOfWeekDate = floor<days>(endOfWeekTime);
	return static_cast<unsigned>(year_month_day{endOfWeekDate}.day());
}

unsigned int GameTime::getLastDayOfCurrentMonth() const
{
	auto currentDate = floor<days>(getPtime(this->ticks));
	auto ymd = year_month_day{currentDate};

	auto lastDayOfMonth = sys_days{ymd.year() / ymd.month() / last};
	return static_cast<unsigned>(year_month_day{lastDayOfMonth}.day());
}

unsigned int GameTime::getDay() const { return (this->ticks + TICKS_PER_DAY) / TICKS_PER_DAY; }

unsigned int GameTime::getMonthDay() const
{
	auto currentDate = floor<days>(getPtime(this->ticks));
	return static_cast<unsigned>(year_month_day{currentDate}.day());
}

unsigned int GameTime::getHours() const
{
	auto timePoint = getPtime(this->ticks);
	auto sinceMidnight = timePoint - floor<days>(timePoint);
	hours h = duration_cast<hours>(sinceMidnight);
	return static_cast<unsigned int>(h.count());
}

unsigned int GameTime::getMinutes() const
{
	auto timePoint = getPtime(this->ticks);
	auto sinceMidnight = timePoint - floor<days>(timePoint);
	minutes m = duration_cast<minutes>(sinceMidnight);
	auto minutesPart = m.count() % 60;
	return static_cast<unsigned int>(minutesPart);
}

unsigned int GameTime::getSeconds() const
{
	auto timePoint = getPtime(this->ticks);
	auto sinceMidnight = timePoint - floor<days>(timePoint);
	seconds s = duration_cast<seconds>(sinceMidnight);
	auto secondsPart = s.count() % 60;
	return static_cast<unsigned int>(secondsPart);
}

unsigned int GameTime::getTicksBetween(unsigned int fromDays, unsigned int fromHours,
                                       unsigned int fromMinutes, unsigned int fromSeconds,
                                       unsigned int toDays, unsigned int toHours,
                                       unsigned int toMinutes, unsigned int toSeconds) const
{
	if (fromDays <= toDays && fromHours <= toHours && fromMinutes <= toMinutes &&
	    fromSeconds < toSeconds)
	{
		unsigned int days_diff_in_ticks = (toDays - fromDays) * TICKS_PER_DAY;
		unsigned int hours_diff_in_ticks = (toHours - fromHours) * TICKS_PER_HOUR;
		unsigned int minutes_diff_in_ticks = (toMinutes - fromMinutes) * TICKS_PER_MINUTE;
		unsigned int seconds_diff_in_ticks = (toSeconds - fromSeconds) * TICKS_PER_SECOND;

		return days_diff_in_ticks + hours_diff_in_ticks + minutes_diff_in_ticks +
		       seconds_diff_in_ticks;
	}
	else
		return 0;
}

uint64_t GameTime::getTicks() const { return ticks; }

bool GameTime::secondPassed() const { return secondPassedFlag; }

bool GameTime::fiveMinutesPassed() const { return fiveMinutesPassedFlag; }

bool GameTime::hourPassed() const { return hourPassedFlag; }

bool GameTime::dayPassed() const { return dayPassedFlag; }

bool GameTime::weekPassed() const { return weekPassedFlag; }

void GameTime::clearFlags()
{
	secondPassedFlag = false;
	fiveMinutesPassedFlag = false;
	hourPassedFlag = false;
	dayPassedFlag = false;
	weekPassedFlag = false;
}

void GameTime::addTicks(uint64_t ticks)
{
	this->ticks += ticks;
	uint64_t secondTicks = this->ticks % (TICKS_PER_SECOND);
	uint64_t fiveMinutesTicks = this->ticks % (5 * TICKS_PER_MINUTE);
	if (fiveMinutesTicks < ticks)
	{
		secondPassedFlag = true;
		fiveMinutesPassedFlag = true;
		uint64_t hourTicks = this->ticks % TICKS_PER_HOUR;
		if (hourTicks < ticks)
		{
			hourPassedFlag = true;
			uint64_t dayTicks = this->ticks % TICKS_PER_DAY;
			if (dayTicks < ticks)
			{
				uint64_t days = this->ticks / TICKS_PER_DAY;
				dayPassedFlag = true;
				// game starts on Tuesday, so week rolls on day 6
				if (days % 7 == 6)
				{
					weekPassedFlag = true;
				}
			}
		}
	}
	else
	{
		if (secondTicks < ticks)
		{
			secondPassedFlag = true;
		}
	}
}

GameTime GameTime::midday() { return GameTime(TICKS_PER_HOUR * 12); }
} // namespace OpenApoc
