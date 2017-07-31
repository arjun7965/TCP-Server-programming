struct message {
	int num;
};

/*
 *  converting to string to avoid problems such as endianness problem
 */

int my_send(int num, int  socket) {
        char buffer[1024] = {0};
        sprintf(buffer,"%d", num);
	if(strlen(buffer) < 1024) {
        	send(socket , buffer, 1024, 0 );
	} else {
		printf("ERROR : Larger number than max message buffer");
	}
}

int my_recieve(int new_socket) {
        int num;
        char buffer[1024] = {0};

        read(new_socket , buffer, 1024);
        sscanf(buffer,"%d", &num);
        return num;
}
