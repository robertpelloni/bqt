package widgets

import (
	"testing"

	"gioui.org/layout"
	"gioui.org/op"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

func TestTrayIconInitialization(t *testing.T) {
	tray := NewTrayIcon()
	if tray == nil {
		t.Fatal("Expected NewTrayIcon to return non-nil instance")
	}

	if tray.StatusText != "ONLINE" {
		t.Errorf("Expected initial status ONLINE, got %s", tray.StatusText)
	}

	if !tray.serverRunning {
		t.Errorf("Expected serverRunning to initially be true")
	}
}

func TestTrayIconCallbacks(t *testing.T) {
	tray := NewTrayIcon()

	quitCalled := false
	restartCalled := false
	muteCalled := false
	meshCalled := false

	tray.OnQuitServer = func() { quitCalled = true }
	tray.OnRestartEventLoop = func() { restartCalled = true }
	tray.OnMuteDSP = func() { muteCalled = true }
	tray.OnDisconnectMesh = func() { meshCalled = true }

	var ops op.Ops
	gtx := layout.Context{Ops: &ops}
	th := theme.GetTheme(theme.Cyberpunk)

	// Open tray menu
	tray.MenuVisible = true

	// Simulate rendering menu
	tray.LayoutMenu(gtx, th)

	// Simulate trigger
	tray.OnMuteDSP()
	if !muteCalled {
		t.Errorf("Expected OnMuteDSP to be invoked")
	}

	tray.OnDisconnectMesh()
	if !meshCalled {
		t.Errorf("Expected OnDisconnectMesh to be invoked")
	}

	tray.OnRestartEventLoop()
	if !restartCalled {
		t.Errorf("Expected OnRestartEventLoop to be invoked")
	}

	tray.OnQuitServer()
	if !quitCalled {
		t.Errorf("Expected OnQuitServer to be invoked")
	}
}
