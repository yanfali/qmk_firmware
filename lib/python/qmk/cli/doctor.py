"""QMK Python Doctor

Check up for QMK environment.
"""
import shutil
import platform
import os

from milc import cli


@cli.entrypoint('Basic QMK environment checks')
def main(cli):
    """Basic QMK environment checks.

    This is currently very simple, it just checks that all the expected binaries are on your system.

    TODO(unclaimed):
        * [ ] Run the binaries to make sure they work
        * [ ] Compile a trivial program with each compiler
        * [ ] Check for udev entries on linux
    """

    binaries = ['dfu-programmer', 'avrdude', 'dfu-util', 'avr-gcc', 'arm-none-eabi-gcc']

    cli.log.info(
        """
{fg_green}  ____    __  _____ __  ___           __
{fg_green} / __ \  /  |/  / //_/ / _ \___  ____/ /____  ____
{fg_green}/ /_/ / / /|_/ / ,<   / // / _ \/ __/ __/ _ \/ __/
{fg_green}\___\_\/_/  /_/_/|_| /____/\___/\__/\__/\___/_/   is in the house"""
    )

    ok = True
    for binary in binaries:
        res = shutil.which(binary)
        if res is None:
            cli.log.error('{fg_red}QMK can\'t find ' + binary + ' in your path')
            ok = False

    OS = platform.system()
    if OS == "Darwin":
        cli.log.info("Detected {fg_cyan}macOS")
    elif OS == "Linux":
        cli.log.info("Detected {fg_cyan}linux")
        test = 'systemctl list-unit-files | grep enabled | grep -i ModemManager'
        if os.system(test) == 0:
            cli.log.warn("{bg_yellow}Detected modem manager. Please disable it if you are using Pro Micros")
    else:
        cli.log.info("Assuming {fg_cyan}Windows")

    if ok:
        cli.log.info('{fg_green}QMK is ready to go')
