package ui

import (
	"sync"
	"testing"
	"time"

	"github.com/robertpelloni/bqt/internal/ui/widgets"
)

func TestWidgetsEventLoopIntegration(t *testing.T) {
	el := GetEventLoop()
	el.Stop()

	go el.Run()
	defer el.Stop()

	tray := widgets.NewTrayIcon()

	var wg sync.WaitGroup
	var onceQuit, onceRestart, onceMute, onceDisconnect sync.Once

	quitSignal := NewSignal("TrayQuit")
	restartSignal := NewSignal("TrayRestart")
	muteSignal := NewSignal("TrayMute")
	disconnectSignal := NewSignal("TrayDisconnect")

	quitHandled := false
	restartHandled := false
	muteHandled := false
	disconnectHandled := false

	wg.Add(4)

	quitSignal.Connect(func(args ...interface{}) {
		quitHandled = true
		onceQuit.Do(func() { wg.Done() })
	})

	restartSignal.Connect(func(args ...interface{}) {
		restartHandled = true
		onceRestart.Do(func() { wg.Done() })
	})

	muteSignal.Connect(func(args ...interface{}) {
		muteHandled = true
		onceMute.Do(func() { wg.Done() })
	})

	disconnectSignal.Connect(func(args ...interface{}) {
		disconnectHandled = true
		onceDisconnect.Do(func() { wg.Done() })
	})

	// Wire widget callbacks to emit signals over the event loop
	tray.OnQuitServer = func() {
		quitSignal.Emit()
	}
	tray.OnRestartEventLoop = func() {
		restartSignal.Emit()
	}
	tray.OnMuteDSP = func() {
		muteSignal.Emit()
	}
	tray.OnDisconnectMesh = func() {
		disconnectSignal.Emit()
	}

	// Trigger widget actions
	tray.OnMuteDSP()
	tray.OnDisconnectMesh()
	tray.OnRestartEventLoop()
	tray.OnQuitServer()

	done := make(chan struct{})
	go func() {
		wg.Wait()
		close(done)
	}()

	select {
	case <-done:
		// All signals dispatched through EventLoop and slots executed
	case <-time.After(2 * time.Second):
		t.Fatalf("timed out waiting for widget event loop signal processing")
	}

	if !quitHandled || !restartHandled || !muteHandled || !disconnectHandled {
		t.Errorf("expected all widget event loop signals to be handled (quit=%v, restart=%v, mute=%v, disconnect=%v)",
			quitHandled, restartHandled, muteHandled, disconnectHandled)
	}
}
