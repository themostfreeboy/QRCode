@rem  Add the ZBar installation directory to the path
@rem  so programs may be run from the command prompt
@set PATH=%PATH%;d:\Program Files\ZBar\bin
@cd /D d:\Program Files\ZBar
@echo For basic command instructions type:
@echo     zbarcam --help
@echo     zbarimg --help
@echo Try running:
@echo     zbarimg -d examples\barcode.png
