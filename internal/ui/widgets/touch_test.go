package widgets

import (
	"image"
	"strings"
	"testing"

	"gioui.org/io/pointer"
	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/unit"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

func TestTouchAreaSwipeDetectionAndLayout(t *testing.T) {
	ta := &TouchArea{
		Label:             "Test Gesture Zone",
		AllowMouseAsTouch: true,
	}

	var ops op.Ops
	gtx := layout.Context{
		Ops: &ops,
		Constraints: layout.Exact(image.Point{
			X: 400,
			Y: 200,
		}),
		Metric: unit.Metric{PxPerDp: 1.0, PxPerSp: 1.0},
	}

	th := theme.GetTheme(theme.Dark)

	// Verify layout sets defaults
	dims := ta.Layout(gtx, th)
	if ta.MinSwipeDistance != 24 {
		t.Fatalf("expected MinSwipeDistance fallback 24, got %f", ta.MinSwipeDistance)
	}
	if dims.Size.X <= 0 || dims.Size.Y <= 0 {
		t.Fatalf("expected non-zero layout dimensions, got %+v", dims.Size)
	}

	// Verify swipe detection math: Swipe Right
	ta.DeltaX = 50
	ta.DeltaY = 0
	dir, dist := ta.detectSwipe()
	if dir != SwipeRight || dist != 50 {
		t.Fatalf("expected SwipeRight with dist 50, got dir=%v, dist=%f", dir, dist)
	}

	// Verify swipe detection math: Swipe Left
	ta.DeltaX = -60
	ta.DeltaY = 0
	dir, dist = ta.detectSwipe()
	if dir != SwipeLeft || dist != 60 {
		t.Fatalf("expected SwipeLeft with dist 60, got dir=%v, dist=%f", dir, dist)
	}

	// Verify swipe detection math: Swipe Down
	ta.DeltaX = 0
	ta.DeltaY = 80
	dir, dist = ta.detectSwipe()
	if dir != SwipeDown || dist != 80 {
		t.Fatalf("expected SwipeDown with dist 80, got dir=%v, dist=%f", dir, dist)
	}

	// Verify swipe detection math: Swipe Up
	ta.DeltaX = 0
	ta.DeltaY = -70
	dir, dist = ta.detectSwipe()
	if dir != SwipeUp || dist != 70 {
		t.Fatalf("expected SwipeUp with dist 70, got dir=%v, dist=%f", dir, dist)
	}

	// Verify finishGesture invokes callback
	var receivedDir SwipeDirection
	var receivedDist float32
	ta.OnSwipe = func(d SwipeDirection, dist float32) {
		receivedDir = d
		receivedDist = dist
	}
	ta.finishGesture()

	if receivedDir != SwipeUp || receivedDist != 70 {
		t.Fatalf("expected callback with SwipeUp 70, got dir=%v, dist=%f", receivedDir, receivedDist)
	}
}

func TestTouchAreaHelpers(t *testing.T) {
	if SwipeNone.String() != "none" {
		t.Errorf("expected 'none', got %q", SwipeNone.String())
	}
	if SwipeLeft.String() != "left" {
		t.Errorf("expected 'left', got %q", SwipeLeft.String())
	}
	if SwipeRight.String() != "right" {
		t.Errorf("expected 'right', got %q", SwipeRight.String())
	}
	if SwipeUp.String() != "up" {
		t.Errorf("expected 'up', got %q", SwipeUp.String())
	}
	if SwipeDown.String() != "down" {
		t.Errorf("expected 'down', got %q", SwipeDown.String())
	}

	ta := &TouchArea{
		Label:             "Workspace",
		AllowMouseAsTouch: false,
	}
	if ta.headerText() != "Workspace (touch only)" {
		t.Errorf("expected 'Workspace (touch only)', got %q", ta.headerText())
	}

	ta.AllowMouseAsTouch = true
	if ta.headerText() != "Workspace (touch + mouse fallback)" {
		t.Errorf("expected 'Workspace (touch + mouse fallback)', got %q", ta.headerText())
	}

	// Verify acceptSource
	ta.AllowMouseAsTouch = false
	if !ta.acceptSource(pointer.Touch) {
		t.Errorf("expected TouchArea to accept pointer.Touch")
	}
	if ta.acceptSource(pointer.Mouse) {
		t.Errorf("expected TouchArea to reject pointer.Mouse when AllowMouseAsTouch is false")
	}

	ta.AllowMouseAsTouch = true
	if !ta.acceptSource(pointer.Mouse) {
		t.Errorf("expected TouchArea to accept pointer.Mouse when AllowMouseAsTouch is true")
	}

	// Verify sourceString
	if ta.sourceString(pointer.Touch) != "touch" {
		t.Errorf("expected 'touch', got %q", ta.sourceString(pointer.Touch))
	}
	if ta.sourceString(pointer.Mouse) != "mouse" {
		t.Errorf("expected 'mouse', got %q", ta.sourceString(pointer.Mouse))
	}
	if ta.sourceString(pointer.Source(255)) != "unknown" {
		t.Errorf("expected 'unknown', got %q", ta.sourceString(pointer.Source(255)))
	}

	// Verify statusText
	idleStatus := ta.statusText()
	if !strings.Contains(idleStatus, "state=idle") {
		t.Errorf("expected idle status text, got %q", idleStatus)
	}

	ta.Active = true
	ta.LastSource = pointer.Touch
	ta.DeltaX = 20
	ta.DeltaY = -15
	activeStatus := ta.statusText()
	if !strings.Contains(activeStatus, "state=active source=touch") {
		t.Errorf("expected active status text, got %q", activeStatus)
	}
}
