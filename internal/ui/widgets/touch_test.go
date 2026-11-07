package widgets

import (
	"image"
	"testing"

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
