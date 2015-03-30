//we assume each error is 4 bits
unsigned int lift_error(int error, int new_error) {
	error = error << 4;
	error = error + new_error
}