from practicum import find_mcu_boards
from peri import McuWithPeriBoard
from time import sleep

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

# count = 0
while True:
    # b.setLedValue(count)
    sw = b.getSwitch()
    have_password = False
    # light = b.getLight()
    password = 'UNKNOWN'
    if sw is True and not have_password:
        have_password = True
        password = ''.join([str(randint(0,9)) for i in range(randint(6,9))])
    print('Your password is {}'.format(password))
    if inp == input():
        password = 'UNKNOWN'
        have_password = False
    # else:
    #     state = "RELEASED"

    # print("LEDs set to %d | Switch state: %-8s | Light value: %d" % (
    #         count, state, light))

    # count = (count + 1) % 8
    sleep(0.5)

