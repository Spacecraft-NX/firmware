all:
	@$(MAKE) --no-print-directory -C libs/CMSIS $(MAKECMDGOALS) -$(MAKEFLAGS)
	@$(MAKE) --no-print-directory -C libs/GD32F3x0_standard_peripheral $(MAKECMDGOALS) -$(MAKEFLAGS)
	@$(MAKE) --no-print-directory -C libs/GD32F3x0_usbfs_driver $(MAKECMDGOALS) -$(MAKEFLAGS)
	@$(MAKE) --no-print-directory -C bootloader $(MAKECMDGOALS) -$(MAKEFLAGS)
	@$(MAKE) --no-print-directory -C bootloader_updater $(MAKECMDGOALS) -$(MAKEFLAGS)
	@$(MAKE) --no-print-directory -C firmware $(MAKECMDGOALS) -$(MAKEFLAGS)

clean:
	@$(MAKE) --no-print-directory -C libs/CMSIS $(MAKECMDGOALS) -$(MAKEFLAGS) clean
	@$(MAKE) --no-print-directory -C libs/GD32F3x0_standard_peripheral $(MAKECMDGOALS) -$(MAKEFLAGS) clean
	@$(MAKE) --no-print-directory -C libs/GD32F3x0_usbfs_driver $(MAKECMDGOALS) -$(MAKEFLAGS) clean
	@$(MAKE) --no-print-directory -C bootloader $(MAKECMDGOALS) -$(MAKEFLAGS) clean
	@$(MAKE) --no-print-directory -C bootloader_updater $(MAKECMDGOALS) -$(MAKEFLAGS) clean
	@$(MAKE) --no-print-directory -C firmware $(MAKECMDGOALS) -$(MAKEFLAGS) clean
