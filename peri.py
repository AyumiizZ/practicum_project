from practicum import McuBoard,find_mcu_boards
from random import randint
RQ_SET_LED    = 0
RQ_GET_SWITCH = 1
RQ_GET_LIGHT  = 2

####################################
class McuWithPeriBoard(McuBoard):

    ################################
    def setLed(self, led_no, led_val):
        '''
        Set status of LED led_no on peripheral board to led_val
        '''
        self.usb_write(request=0,index=led_no,value=led_val)

    ################################
    def setLedValue(self, value):
        '''
        Display value's 3 LSBs on peripheral board's LEDs
        '''
        # binary = '{:03}'.format(int(bin(value)[2:]))
        return self.usb_write(request=1,value=value)
        

    ################################
    def getSwitch(self):
        '''
        Return a boolean value indicating whether the switch on the peripheral
        board is currently pressed
        '''
        # return ''.join([randint(0,9) for i in range(randint(6,9))]) 
        return bool(self.usb_read(request=2,length=1)[0])

    ################################
    def getLight(self):
        '''
        Return the current reading of light sensor on peripheral board
        '''
        res = self.usb_read(request=3,length=2)
        return (res[1]<<8)+res[0]
