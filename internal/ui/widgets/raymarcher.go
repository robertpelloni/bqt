package widgets

import (
	"image"
	"math"
	"sync"
	"gioui.org/layout"
	"gioui.org/op/paint"
)

type Raymarcher struct {
	Time float64
}

func (r *Raymarcher) Layout(gtx layout.Context) layout.Dimensions {
	w, h := gtx.Constraints.Max.X/4, gtx.Constraints.Max.Y/4
	if w <= 0 || h <= 0 { return layout.Dimensions{} }

	img := image.NewRGBA(image.Rect(0, 0, w, h))
	
	// --- GO PARALLEL PIXEL DISPATCHER ---
	var wg sync.WaitGroup
	for y := 0; y < h; y++ {
		wg.Add(1)
		go func(y int) {
			defer wg.Done()
			for x := 0; x < w; x++ {
				// 3D SDF Math Ported to Go
				uvX := (2.0*float64(x) - float64(w)) / float64(h)
				uvY := (2.0*float64(y) - float64(h)) / float64(h)
				
				// Simplified Sphere SDF for Parity
				dist := math.Sqrt(uvX*uvX + uvY*uvY) - (0.5 + 0.1*math.Sin(r.Time))
				
				if dist < 0 {
					img.Set(x, y, image.RGBAColor{R: 0, G: 240, B: 255, A: 255})
				} else {
					img.Set(x, y, image.RGBAColor{R: 10, G: 10, B: 15, A: 255})
				}
			}
		}(y)
	}
	wg.Wait()

	paint.NewImageOp(img).Add(gtx.Ops)
	paint.PaintOp{}.Add(gtx.Ops)

	return layout.Dimensions{Size: gtx.Constraints.Max}
}
