# Defining a function
- First print the label
- Save the current frame pointer at -4
- Save the current return address at -8
- Load each of the current vaiables into the stack
- Make the f


# Logical And
- First evaluate the left hand operator and place in register
- Compare the value to 0 and place response on stack
- Take value off stack and place in register
- If value on stack is 1 jump to fail flag
- Repeat for second operator
- If value is 1 jump to fail flag
- Place a 1 on stack
- jump to end flag
- Fail Flag:
- Place a 0 on stack
- End Flag: