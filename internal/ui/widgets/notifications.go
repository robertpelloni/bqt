package widgets

import (
	"image"
	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/op/clip"
	"gioui.org/op/paint"
	"gioui.org/unit"
	"gioui.org/widget/material"
	"github.com/robertpelloni/bobui/internal/ui/theme"
	"time"
	"sync"
)

type Toast struct {
	Title, Msg string
	Type       string
	Expiry     time.Time
}

type NotificationCenter struct {
	mu     sync.Mutex
	toasts []Toast
}

var (
	notifyInstance *NotificationCenter
	notifyOnce     sync.Once
)

func GetNotificationCenter() *NotificationCenter {
	notifyOnce.Do(func() {
		notifyInstance = &NotificationCenter{}
	})
	return notifyInstance
}

func (nc *NotificationCenter) ShowToast(title, msg, tType string) {
	nc.mu.Lock()
	defer nc.mu.Unlock()
	nc.toasts = append(nc.toasts, Toast{
		Title: title, Msg: msg, Type: tType,
		Expiry: time.Now().Add(3 * time.Second),
	})
}

func (nc *NotificationCenter) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	nc.mu.Lock()
	defer nc.mu.Unlock()

	// Filter expired toasts
	active := make([]Toast, 0)
	for _, t := range nc.toasts {
		if time.Now().Before(t.Expiry) { active = append(active, t) }
	}
	nc.toasts = active

	// --- GO NOTIFICATION RENDER PASS ---
	// Render toasts in the top-right corner using Gio layouting.
	return layout.NW.Layout(gtx, func(gtx layout.Context) layout.Dimensions {
		return layout.Flex{Axis: layout.Vertical}.Layout(gtx,
			layout.Rigid(func(gtx layout.Context) layout.Dimensions {
				for _, t := range nc.toasts {
					// Draw Toast Background
					rect := image.Rect(0, 0, gtx.Dp(unit.Dp(300)), gtx.Dp(unit.Dp(60)))
					stack := clip.Rect(rect).Push(gtx.Ops)
					paint.Fill(gtx.Ops, th.Surface)
					stack.Pop()
					
					// Draw Title (Simulated Material Label)
					lbl := material.Label(material.NewTheme(), unit.Sp(12), t.Title)
					lbl.Color = th.Primary
					lbl.Layout(gtx)
				}
				return layout.Dimensions{}
			}),
		)
	})
}
