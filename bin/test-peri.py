from practicum import find_mcu_boards
from peri import McuWithPeriBoard
from time import sleep
from random import randint

devs = find_mcu_boards()

if len(devs) == 0:
    print("*** No practicum board found.")
    exit(1)

b = McuWithPeriBoard(devs[0])
print("*** Practicum board found")
print("*** Manufacturer: %s" % \
        b.handle.getString(b.device.iManufacturer, 256))
print("*** Product: %s" % \
        b.handle.getString(b.device.iProduct, 256))

password = 'UNKNOWN'
have_password = False
lock = False
count = 0
while True:
    # b.setLedValue(count)
    sw = b.getSwitch()
    # light = b.getLight()
    if sw is True and not have_password:
        have_password = True
        #password = ''.join([str(randint(0,9)) for i in range(randint(6,9))])
        password = ''.join([str(randint(0,9)) for i in range(randint(4, 6))])
    if have_password and lock:
        print('Your password is {}'.format(password))
        inp = input()
        if password == inp:
            lock = False
            print('Unlocked')
            password = 'UNKNOWN'
            have_password = False
            count = 0
        else:
            count += 1
            print('Failed {} times'.format(count))
    elif have_password and not lock:
        print('Locked')
        lock = True
        sleep(0.5)
    if count > 3:
        cd = 2**(count-3)
        print(f'Failed {count} time(s)')
        for i in range(cd,0,-1):
            print(f'Sleep {i} secs left')
            sleep(60)

    # else:
    #     state = "RELEASED"

    # print("LEDs set to %d | Switch state: %-8s | Light value: %d" % (
    #         count, state, light))

    # count = (count + 1) % 8
    sleep(0.2)

