import sys;

if (len(sys.argv) != 4):
    exit("Invalid argument count. arguments should be: <dataName> <inputFile> <outputFile>")
data_name, input_fn, output_fn = sys.argv[1:]

fin  = open(input_fn,'r')
fout = open(output_fn,'w')

nameslist = [n for n in fin.read().split('\n') if len(n)]
names = '",\n\t"'.join(nameslist)
print(f'const char* {data_name}[{len(nameslist)}]{{\n\t"{names}"\n}};',file=fout)