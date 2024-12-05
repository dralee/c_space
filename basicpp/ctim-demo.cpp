/*
 * the ctime library has a variety of functions that allow you to measure dates and times.
 * 2024.12.05 by darlee
 *
 *
Format Specifier	Description	Example
%a	Short representation of the weekday	Fri
%A	Full representation of the weekday	Friday
%b	Short representation of the month name	Dec
%B	Full representation of the month name	December
%c	Full date and time representation	Fri Dec 17 14:30:01 2023
%C	Century (equivalent to taking the first two digits of a 4-digit year)	20
%d	Day of the month with leading zero	09
%D	Date representation equivalent to %m/%d/%y	12/17/23
%e	Day of the month with leading spaces	 9
%F	Date representation equivalent to %Y-%m-%d	2023-12-17
%g	2-digit week-based year (week-based years start at the beginning of a week)	23
%G	4-digit week-based year (week-based years start at the beginning of a week)	2023
%h	Short representation of the month name (equivalent to %b)	Dec
%H	24-hour format of an hour	14
%I	12-hour format of an hour	02
%j	Day of the year (from 0 through 365)	351
%m	Numeric representation of a month	351
%M	Minutes within an hour	30
%n	A \n new line character
%p	AM or PM	PM
%r	Full 12-hour time format	02:30:01 PM
%R	24-hour time format equivalent to %H:%M	14:30
%S	Seconds within a minute	01
%t	A \t tab character
%T	Full 24-hour time format equivalent to %H:%M:%S	14:30:01
%u	Numeric representation of a day of the week (from 1 to 7 starting with Monday)	7
%U	Week of the year starting at 0, with week 1 beginning on the first Sunday of the year	51
%V	Week of the year starting at 1, with week 1 beginning on the first Monday of the year and any day in January before the first Monay belonging to the previous year	50
%w	Numeric representation of a day of the week (from 0 to 6 starting with Sunday)	0
%W	Week of the year starting at 0, with week 1 beginning on the first Monday of the year	50
%x	Locale-based date representation	12/17/23
%X	Locale-based time representation	14:30:01
%y	2-digit year representation	23
%Y	4-digit year representation	2023
%z	Numeric timezone offset	+0000
%Z	Timezone name	GMT
%%	A % character	%
 */
#include <iostream>
#include <ctime>
using namespace std;

void demo_1(){
	// returns a C-style string representation of the time in a tm structure
	time_t timestamp;
	time(&timestamp);
	struct tm *time = localtime(&timestamp);
	cout<<asctime(time)<<endl;
	// returns a C-style string representation of the time in a timestamp
	cout<<ctime(&timestamp)<<endl;

	// returns a number representing the amount of time that has passed while the program is running
	clock_t before = clock();
	int k = 0;
	for(int i = 0; i < 100000; ++i){
		k += i;
	}
	clock_t duration = clock() - before;
	cout<<"Duration: "<<(float)duration/CLOCKS_PER_SEC<<" seconds"<<endl;



}


void demo_2(){
	// returns the time difference between two timestamps
	struct tm date;
	time_t now, before;
	
	time(&now);

	date = *localtime(&now);
	date.tm_hour -= 6;
	before = mktime(&date);

	cout<<"difference time:"<<difftime(now, before)<<endl;

	// converts a timestamp into a tm structure representing its time at the GMT time zone
	time_t now1;
	struct tm *date1;
	time(&now);
	date1 = gmtime(&now);

	cout<<"GMT time: "<<asctime(date1)<<endl;

}

void demo_3(){
	// converts a timestamp into a tm structure representing its time in the system's local time zone
	time_t now;
	struct tm *date;
	time(&now);
	date = localtime(&now);
	cout<<"local time:"<<asctime(date);

	// create a tiemstamp and print its date and time
	struct tm date1;
	time_t timestamp;

	date1.tm_year = 2024 - 1900; // number of years since 1900
	date1.tm_mon = 12 -1;
	date1.tm_mday = 5;
	date1.tm_hour = 17;
	date1.tm_min = 45;
	date1.tm_sec = 12;
	date1.tm_isdst = -1;

	timestamp = mktime(&date1);
	cout<<ctime(&timestamp);

	// writes a C-style string representing the date and time of a tm structure with a variety of formatting options
	time_t timestamp2;
	char output[50];
	struct tm *datetime;
	time(&timestamp2);
	datetime = localtime(&timestamp);
	strftime(output, 50, "%B %e, %Y", datetime);
	cout<<output<<endl;

	strftime(output, 50, "%I:%M:%S %p", datetime);
	cout<<output<<endl;

	strftime(output, 50, "%D", datetime);
	cout<<output<<endl;
	
	strftime(output, 50, "%c", datetime);
	cout<<output<<endl;

}

int main(int argc, char **argv){
	demo_1();
	demo_2();
	demo_3();
	return 0;
}
