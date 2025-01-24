import sys
from decimal import Context, Decimal, setcontext

setcontext(Context(prec=2000000, Emax=2000000, Emin=0))
print((Decimal(sys.stdin.readline()) * Decimal(sys.stdin.readline())))
