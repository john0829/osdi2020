file ./build/kernel8.elf
target remote :1234
b mm.c:245
watch 
continue