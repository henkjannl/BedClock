
    # F    =    (1-x)^3*A + 3*x*(1-x)^2*B             + 3*x^2*(1-x)*C+x^3*D
    # F*dX = -3*(1-x)^2*A + {-3*x*2*(1-x)+3*(1-x)^2}*B + {-3*x^2+6*x*(1-x)}*C + 3*x^2*D

    # F (0) =    A
    # F'(0) = -3*A + 3*B
    # F (1) =    D
    # F'(1) = -3*C + 3*D

    # B = 1/3*F'(0)+F0
    # F'(1) = -3*C + 3*F(1)
    # -3*C = 3*F(1) - F'(1)
    # C = F(1) - 1/3*F'(1)

    # F0 = 1.0

F0 = 1.2
F1 = 1
FF0 = 0
FF1 = 0
CELL = "$C66"

F  = lambda x :  F0*(1-x)**3 + (F0+FF0/3)*3*x*(1-x)**2 + (F1-FF1/3)*3*x**2*(1-x) + F1*x**3
FF = lambda x : (F(x+0.001)-F(x))/0.001

print( f"return {F0}*pow(1-x,3) + {3*F0+FF0}*x*pow(1-x,2) + {3*F1-FF1}*pow(x,2)*(1-x) + {F1}*pow(x,3);")
print( f"={F0}*(1-{CELL})^3+{3*F0+FF0}*{CELL}*(1-{CELL})^2 + {3*F1-FF1}*{CELL}^2*(1-{CELL})+{F1}*{CELL}^3")

print( f"F(0) = {F(0)}")
print( f"F(1) = {F(1)}")
print( f"dF(0)*dx = {FF(0)}")
print( f"dF(1)*dx = {FF(1)}")
