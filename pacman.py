__author__ = 'scobb_000'

SIZE = 19
WALL = "WALL"
EMPTY = "EMPTY"
class RowRecord(object):
    def __init__(self, first, sp):
        self.first = first
        self.sp = sp
    def display(self):
        str = create_starter(self.first)
        symmetric_alteration(str, self.sp, "WALL" if self.first == "EMPTY" else "EMPTY")
        print_list(str)

def main():
    print("uint8_t board[BOARD_SIZE_UD][BOARD_SIZE_LR] = {")
    RowRecord(WALL, []).display()
    RowRecord(EMPTY, [0, 9]).display()
    RowRecord(WALL, [1, 4, 8]).display()
    RowRecord(WALL, [1, 4, 8]).display()
    RowRecord(EMPTY, [0]).display()
    RowRecord(WALL, [1, 4, 6]).display()
    RowRecord(WALL, [4, 8]).display()
    RowRecord(EMPTY, [0, 1, 2, 3, 5]).display()
    RowRecord(WALL, [4, 6]).display()
    RowRecord(EMPTY, [0, 7, 8, 9]).display()
    RowRecord(WALL, [4, 6]).display()
    RowRecord(EMPTY, [0, 1, 2, 3, 5]).display()
    RowRecord(WALL, [4, 6]).display()
    RowRecord(EMPTY, [0, 9]).display()
    RowRecord(WALL, [1, 4, 8]).display()
    RowRecord(EMPTY, [0, 3]).display()
    RowRecord(WALL, [2, 4, 6]).display()
    RowRecord(EMPTY, [0, 4, 9]).display()
    RowRecord(WALL, [1, 8]).display()
    RowRecord(EMPTY, [0]).display()
    RowRecord(WALL, []).display()
    print("};")



def create_starter(entry):
    return [entry for _ in range(SIZE)]

def symmetric_alteration(l, ind, new):
    for i in ind:
        l[i] = new
        l[len(l) - 1 - i] = new

def print_list(l):
    print("{" + ", ".join(l) + "},")




if __name__ == '__main__':
    main()