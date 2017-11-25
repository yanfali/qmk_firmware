#Notes and Observations

## Organization of matrix in QMK

QMK uses a bitmap called matrix to record the current state of the
keyboard matrix. MATRIX_ROW_PINS and MATRIX_COL_PINS control it's
dimensions for a normal keyboard that is connected to the usual pins of
a ATMega32U. The macros expand into the port assigments on the MCU.

If you have inputs from elsewhere, say I2C, then you may need to write
a custom matrix.c which overrides the defaults and provides the hooks
which call your sources and create the matrix which is then used by QMK
to decide on the appropriate output.

The size of the matrix is controlled by the number of rows and columns
you configure.


