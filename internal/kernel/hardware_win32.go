//go:build windows

package kernel

import (
	"syscall"
	"unsafe"
)

var (
	user32                  = syscall.NewLazyDLL("user32.dll")
	procRegisterRawInputDevices = user32.NewProc("RegisterRawInputDevices")
)

type RAWINPUTDEVICE struct {
	UsagePage uint16
	Usage     uint16
	Flags     uint32
	Target    uintptr
}

// RegisterHardwareHooks tapping into Win32 for true device separation.
func RegisterHardwareHooks() error {
	// Mouse & Keyboard Registration
	rid := []RAWINPUTDEVICE{
		{UsagePage: 0x01, Usage: 0x02, Flags: 0, Target: 0}, // Mouse
		{UsagePage: 0x01, Usage: 0x06, Flags: 0, Target: 0}, // Keyboard
	}

	ret, _, err := procRegisterRawInputDevices.Call(
		uintptr(unsafe.Pointer(&rid[0])),
		uintptr(len(rid)),
		uintptr(unsafe.Sizeof(rid[0])),
	)

	if ret == 0 {
		return err
	}
	return nil
}
