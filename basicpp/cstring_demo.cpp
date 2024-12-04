/*
 * to use the cstring lib functions
 * C-style string are different than regualar strings.
 * 2024.12.04 by dralee
 * */
#include <cstring>
#include <iostream>

using namespace std;

void string_1() {
    char str[] = "Hello world!";
    // memchr: get a pointer to the first byte in a block of memory which
    // contains a specified value
    char *ptr = (char *)memchr(str, 'o', 12);
    cout << ptr << endl;

    // memcmp: compare two blocks of memory to see which is greater
    char str1[] = "ABCD";
    char str2[] = "ABCE";
    int cmp = memcmp(str1, str2, 4);
    if (cmp > 0) {
        cout << str1 << " is greater than " << str2 << endl;
    } else if (cmp < 0) {
        cout << str2 << " is greater than " << str1 << endl;
    } else {
        cout << str1 << " is equal to " << str2 << endl;
    }

    // memcpy: copy data from one array to another
    int num1[5] = {10, 20, 30, 40, 50};
    int num2[5];
    memcpy(num1, num2, sizeof(num1));
    for (int i = 0; i < 5; ++i) {
        cout << num1[i] << ":" << num2[i] << endl;
    }

    // memmove: copy four integers from one position to another
    int num3[6] = {10, 20, 30, 40, 50, 60};
    cout << "before memmove:";
    for (int i = 0; i < 6; ++i) {
        cout << num3[i] << " ";
    }
    cout << endl;
    memmove(&num3[0], &num3[2], 4 * sizeof(num3[0]));

    cout << "after memmove:";
    for (int i = 0; i < 6; ++i) {
        cout << num3[i] << " ";
    }
    cout << endl;
}

void string_2() {
    // memset: fill an array with zeroes
    int num1[5] = {10, 20, 30, 40, 50};
    memset(num1, 0, sizeof(num1));
    for (int i = 0; i < 5; ++i) {
        cout << num1[i] << " ";
    }
    cout << endl;

    // strcat: concatenate a string
    char str1[20] = "Hello";
    strcat(str1, " World!");
    cout << str1 << endl;

    // strchr: get a pointer to the first occurrence of a character in a string
    char str2[] = "Hello World";
    char *p = strchr(str2, 'W');
    cout << p << endl;

    // strcmp: compare two C-style strings to see which is greater
    char str3[] = "ABCD";
    char str4[] = "ABCE";
    int cmp = strcmp(str3, str4);
    if (cmp > 0) {
        cout << str3 << " is greater than " << str4 << endl;
    } else if (cmp < 0) {
        cout << str4 << " is greater than " << str3 << endl;
    } else {
        cout << str3 << " is equal to " << str4 << endl;
    }

    cmp = strcoll(str3, str4);
    cout << "strcoll:" << endl;
    if (cmp > 0) {
        cout << str3 << " is greater than " << str4 << endl;
    } else if (cmp < 0) {
        cout << str4 << " is greater than " << str3 << endl;
    } else {
        cout << str3 << " is equal to " << str4 << endl;
    }
}

void string_3() {
    // strcpy: copy data from one string to another
    char str1[] = "Hello World!";
    char str2[30];
    strcpy(str2, str1);
    cout << "strcpy: " << str2 << endl;

    // strcspn: measure the length of the string up to the first punctuation
    // mark
    char str[] = "Learn C++, C#, Java and Python!";
    int pos = strcspn(str, ",.!?");
    cout << "pos: " << pos << endl;

    // strerror: output a variety of error messages
	cout<<"strerror: "<<endl;
    cout << strerror(0) << endl;
    cout << strerror(1) << endl;
    cout << strerror(2) << endl;
    cout << strerror(3) << endl;
    cout << strerror(4) << endl;

	// strlen: measure the length of a C-style string
	cout<<"strlen: "<<endl;
	cout<<strlen(str1)<<endl;
	cout<<sizeof(str1)<<endl;

	// strncat: concatenate part of a string
	char str5[20] = "Hello";
	strncat(str5, " World", 6);
	cout<<"strncat: "<<str5<<endl;

	// strncmp: compare the first 3 characters of two strings
    char str3[] = "ABCD";
    char str4[] = "ABCE";
    int cmp = strncmp(str3, str4, 3);
    if (cmp > 0) {
        cout << str3 << " is greater than " << str4 << endl;
    } else if (cmp < 0) {
        cout << str4 << " is greater than " << str3 << endl;
    } else {
        cout << str3 << " is equal to " << str4 << endl;
    }
	
	// strncpy: copy part of the data from one string to another
	char str6[] = "Hello World!";
	char str7[] = "Write code!";
	strncpy(str7, str6, 6);
	cout<<"strncpy:"<<endl;
	cout<<str6<<endl;
	cout<<str7<<endl;

	// strpbrk: print the part of the string that starts at the first digit
	char str8[] = "I think 4096 bytes should be enough";
	char *ps = strpbrk(str8, "0123456789");
	cout<<ps<<endl;
	
	// strrchr: get a pointer to the last occurrence of a character in a string
	char str9[] = "Hello World";
	char *p1 = strchr(str9, 'o');
	char *p2 = strrchr(str9, 'o');
	cout<<"first: "<<p1<<",last: "<<p2<<endl;

	// strspn: measure the length of the string up to the first non-digit
	char str10[] = "4096 bytes should be enough";
	int pos1 = strspn(str10,"0123456789");
	cout<<pos1<<endl;

	// strstr: get a pointer to the first occurrence of a C-style string in another string
	char str11[] = "The rain in Spain falls mainly on the plains";
	char *p3 = strstr(str11, "ain");
	cout<<p3<<endl;

	// strtok: separate the words in a sentence by using spaces as a delimiter
	char str12[] = "Learn C++ at W3Schools";
	char *p4 = strtok(str12, " ");
	while(p4 != NULL){
		cout<<p4<<endl;
		p4 = strtok(NULL, " ");
	}

	// strxfrm: the length of the transformed string, not including the terminating null-character
	char str13[] = "Hello World";
	char str14[1+strxfrm(NULL,str13,0)];
	strxfrm(str14, str13, sizeof str14);
	cout<<"strxfrm: "<<str14<<endl;

}

int main(int argc, char **argv) {
    string_1();
    string_2();
    string_3();
    return 0;
}
