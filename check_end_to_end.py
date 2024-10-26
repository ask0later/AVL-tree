from subprocess import run, Popen, PIPE
from sys import executable

num_test = 1
is_ok = True
for i in range(1, 6):
    str_data =  "tests/end_to_end/" + str(i) + ".dat"
    file_in = open(str_data, "r")
    str_ans = "tests/end_to_end/" + str(i) + ".ans"

    ans = []
    for i in open(str_ans):
        ans = list(map(int, i.split()))
	
    result = run(["./build/src/main"], capture_output = True, encoding='cp866', stdin=file_in)
    print("Test: " + str(num_test).strip())

    res = list(map(int, result.stdout.split()))

    is_ok &= (res == ans)
    if res == ans:
        print("OK")
    else:
        #print("ERROR\nExpect:", ans, "\nGive:  ", res)
        print("ERROR")
    print("-------------------------------------------------")
    num_test += 1

if is_ok:
	print("TESTS PASSED")
else:
	print("TESTS FAILED")