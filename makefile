all:
	gcc bitmanip.c -o bitmanip -Wall && ./bitmanip 0 8

small:
	gcc -Os -o b_sml -ffunction-sections -fdata-sections -Wl,--gc-sections bitmanip.c
	strip -s -R .comment -R .gnu.version --strip-unneeded b_sml -o bitmanip
	rm b_sml
