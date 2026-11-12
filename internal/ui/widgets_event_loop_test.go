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

func TestTouchAreaEventLoopIntegration(t *testing.T) {
	el := GetEventLoop()
	el.Stop()

	go el.Run()
	defer el.Stop()

	ta := &widgets.TouchArea{
		Label:            "EventLoop Gesture Pad",
		MinSwipeDistance: 20,
	}

	var wg sync.WaitGroup
	var onceTouch, onceSwipe sync.Once

	touchSignal := NewSignal("TouchStateChanged")
	swipeSignal := NewSignal("SwipeDetected")

	touchHandled := false
	swipeHandled := false

	wg.Add(2)

	touchSignal.Connect(func(args ...interface{}) {
		touchHandled = true
		onceTouch.Do(func() { wg.Done() })
	})

	swipeSignal.Connect(func(args ...interface{}) {
		swipeHandled = true
		onceSwipe.Do(func() { wg.Done() })
	})

	ta.OnTouch = func(active bool) {
		touchSignal.Emit(active)
	}
	ta.OnSwipe = func(direction widgets.SwipeDirection, distance float32) {
		swipeSignal.Emit(direction.String(), distance)
	}

	// Trigger callbacks directly
	ta.OnTouch(true)
	ta.OnSwipe(widgets.SwipeRight, 45.0)

	done := make(chan struct{})
	go func() {
		wg.Wait()
		close(done)
	}()

	select {
	case <-done:
		// Successful async dispatch
	case <-time.After(2 * time.Second):
		t.Fatalf("timed out waiting for touch area event loop signal processing")
	}

	if !touchHandled || !swipeHandled {
		t.Errorf("expected touch and swipe signals to be handled, got touch=%v, swipe=%v", touchHandled, swipeHandled)
	}
}
