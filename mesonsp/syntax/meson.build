project('syntaxdemo', 'c')
executable('hello','main.c')

# demo to generate config1.h config2.h
cdata = configuration_data({
  'STRING' : '"foo"',
  'INT' : 42,
  'DEFINED' : true,
  'UNDEFINED' : false,
})

configure_file(output : 'config1.h',
  configuration : cdata,
)

configure_file(output : 'config2.h',
  configuration : {
    'STRING' : '"foo"',
    'INT' : 42,
    'DEFINED' : true,
    'UNDEFINED' : false,
  }
)

# demo from config.h.in to generate the config.h
conf_data = configuration_data()
conf_data.set('version', '1.2.3')
configure_file(input: 'config.h.in',
          output: 'config.h',
          configuration: conf_data)

# variables
var1 = 'hello'
var2 = 102
var3 = [1,2,3]
var4 = var3
var4 += [4]

# number
x = 1 + 2
y = 3 * 4
d = 5 % 3

# hex
int_255 = 0xFF
# octal
int_493 = 0o755
# binary
int_1365 = 0b10101010101

# strings to int
string_var = '42'
num = string_var.to_int()
# number to string
int_var = 42
string_var = int_var.to_string()
# boolean
truth = true
bool_var = true
string_var = bool_var.to_string()
int_var = bool_var.to_int()

# strings quotes
single_quote = 'contains a \' character'
message(single_quote) # show the message

# join
str1 = 'abc'
str2 = 'xyz'
combined = str1 + '_' + str2
message(combined)

# path join
joined = '/usr/share' / 'pname' #=> /usr/share/pname
message(joined)
joined = '/usr/local' / '/etc/name' #=> /etc/name
message(joined)
joined = 'C:\\foo\\bar' / 'pname' #=> C:/foo/bar/pname
message(joined)
joined = 'C:\\foo\\bar' / 'D:\\name' #=> D:/name
message(joined)

# multiple lines
multiline_string = '''#include <foo.h>
int main (int argc, char ** argv) {
  return FOO_SUCCESS;
}'''

message(multiline_string)

# string index
foo = 'abcd'
message(foo[1]) # b

# string format
template = 'string: @0@, number: @1@, bool: @2@'
res = template.format('text',1,true)
message(res)

n = 10
m = 'hi'
s = f'result: @n@ + @m@'
message(s)

# string replace
s = 'semicolons;as;separators'
s = s.replace('as', 'are')
message(s)

# strip
define = ' -Dsomedefine '
stripped_define = define.strip()
message(stripped_define)
string = 'xyxHelloxyx'.strip('xy')
message(string)

# to_upper, to_lower
target = 'x86_FreeBSD'
upper = target.to_upper()
lower = target.to_lower()
message(upper,lower)

# contains starswith,endswith
target = 'x86_FreeBSD'
is_fbsd = target.to_lower().contains('freebsd')

is_x86 = target.startswith('x86') # boolean value 'true'
is_bsd = target.to_lower().endswith('bsd') # boolean value 'true'





