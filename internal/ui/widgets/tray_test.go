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

func TestTrayIconMenuClickActions(t *testing.T) {
	tray := NewTrayIcon()

	var ops op.Ops
	gtx := layout.Context{Ops: &ops}
	th := theme.GetTheme(theme.Cyberpunk)

	// Verify LayoutIcon toggles MenuVisible on click
	if tray.MenuVisible {
		t.Fatal("expected menu to initially be hidden")
	}
	tray.IconClick.Click()
	tray.LayoutIcon(gtx, th)
	if !tray.MenuVisible {
		t.Fatal("expected menu to become visible after icon click")
	}

	// Test Mute button click
	muteFired := false
	tray.OnMuteDSP = func() { muteFired = true }
	tray.btnMute.Click()
	tray.LayoutMenu(gtx, th)
	if !tray.isMuted || !muteFired {
		t.Fatalf("expected isMuted true and mute callback fired, got muted=%v, fired=%v", tray.isMuted, muteFired)
	}

	// Test Disconnect button click
	meshFired := false
	tray.OnDisconnectMesh = func() { meshFired = true }
	tray.btnDisconnect.Click()
	tray.LayoutMenu(gtx, th)
	if tray.isMeshActive || !meshFired {
		t.Fatalf("expected isMeshActive false and mesh callback fired, got active=%v, fired=%v", tray.isMeshActive, meshFired)
	}

	// Test Restart button click
	restartFired := false
	tray.OnRestartEventLoop = func() { restartFired = true }
	tray.btnRestart.Click()
	tray.LayoutMenu(gtx, th)
	if !restartFired {
		t.Fatal("expected restart callback to be fired")
	}

	// Test Quit button click
	quitFired := false
	tray.OnQuitServer = func() { quitFired = true }
	tray.btnQuit.Click()
	tray.LayoutMenu(gtx, th)
	if tray.serverRunning || !quitFired || tray.StatusText != "SHUTTING DOWN" {
		t.Fatalf("expected serverRunning=false, quit callback fired, got running=%v, status=%s", tray.serverRunning, tray.StatusText)
	}
}
