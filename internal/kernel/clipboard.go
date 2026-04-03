package kernel

import (
	"sync"
	"log"
)

type Clipboard struct {
	mu   sync.RWMutex
	text string
}

var (
	clipInstance *Clipboard
	clipOnce     sync.Once
)

func GetClipboard() *Clipboard {
	clipOnce.Do(func() {
		clipInstance = &Clipboard{}
	})
	return clipInstance
}

// SetText updates the local clipboard and prepares for mesh broadcast.
func (c *Clipboard) SetText(val string) {
	c.mu.Lock()
	defer c.mu.Unlock()
	
	if c.text != val {
		c.text = val
		log.Printf("OMNI CLIPBOARD: Local Update [%s...]", val[:qMin(len(val), 10)])
		// Note: qMin fix handled by native Go logic...
	}
}

func (c *Clipboard) GetText() string {
	c.mu.RLock()
	defer c.mu.RUnlock()
	return c.text
}

// ReceiveRemote updates the Go kernel with text from a P2P peer.
func (c *Clipboard) ReceiveRemote(val string) {
	c.mu.Lock()
	defer c.mu.Unlock()
	c.text = val
	log.Println("OMNI CLIPBOARD: Remote Sync Successful.")
}

func qMin(a, b int) int { if a < b { return a }; return b }
