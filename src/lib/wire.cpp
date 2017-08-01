#include "wire.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>

Wire::Wire(int bus) {
	this->_bus = bus;
	this->_fd = -1;
	this->_addr = -1;
	this->open();
}

Wire::~Wire() {
	if(this->_fd >= 0)
		this->close();
}

int Wire::open() {
	if(this->_fd >= 0)
		return -1;
	// /dev/i2c-1
	char* dev = new char[12];
	sprintf(dev, "/dev/i2c-%d", this->_bus);
	this->_fd = ::open(dev, O_RDWR);
	delete[] dev;
	return this->_fd>=0?0:-2;
}

void Wire::close() {
	if(this->_fd < 0)
		return;
	::close(this->_fd);
}

int Wire::begin(byte addr) {
	if(this->_fd < 0 || this->_addr == addr)
		return -1;
	int res = ioctl(this->_fd, I2C_SLAVE, addr);
	if(res >= 0)
		this->_addr = addr;
	return res>=0?0:-2;
}

int Wire::write(byte val) {
	if(this->_fd < 0)
		return -1;
	return ::write(this->_fd, &val, 1)==1?0:-2;
}

int Wire::end() {
	if(this->_addr >= 0) {
		this->_addr = -1;
		return 0;
	}
	return -1;
}
