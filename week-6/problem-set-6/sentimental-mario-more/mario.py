from cs50 import get_int

# Prompt the user for the half-pyramid’s height
while True:
    height = get_int('Height: ')
    if height > 0 and height < 9:
        break

# Generate the desired half-pyramids
for i in range(1, (height + 1)):
    spaces = ' ' * (height - i)
    blocks = '#' * i
    print(f'{spaces}{blocks}  {blocks}')
