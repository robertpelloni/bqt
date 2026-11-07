package widgets

import (
	"image"
	"testing"
	"time"

	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/unit"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

func TestNotificationCenterLifecycleAndLayout(t *testing.T) {
	nc := GetNotificationCenter()
	if nc == nil {
		t.Fatal("expected non-nil notification center singleton")
	}

	// Clear any residual toasts for test isolation
	nc.mu.Lock()
	nc.toasts = nil
	nc.mu.Unlock()

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

	// When empty, should return zero dimensions
	dimsEmpty := nc.Layout(gtx, th)
	if dimsEmpty.Size.X != 0 || dimsEmpty.Size.Y != 0 {
		t.Fatalf("expected 0x0 for empty notifications, got %+v", dimsEmpty.Size)
	}

	// Show toast
	nc.ShowToast("Update Available", "BQt v1.1.98 is ready", "info")

	dimsRendered := nc.Layout(gtx, th)
	if dimsRendered.Size.X <= 0 || dimsRendered.Size.Y <= 0 {
		t.Fatalf("expected non-zero dimensions for active toast, got %+v", dimsRendered.Size)
	}

	// Test toast expiration
	nc.mu.Lock()
	if len(nc.toasts) > 0 {
		nc.toasts[0].Expiry = time.Now().Add(-1 * time.Second) // Force expire
	}
	nc.mu.Unlock()

	dimsExpired := nc.Layout(gtx, th)
	if dimsExpired.Size.X != 0 || dimsExpired.Size.Y != 0 {
		t.Fatalf("expected 0x0 after toast expiration, got %+v", dimsExpired.Size)
	}
}
