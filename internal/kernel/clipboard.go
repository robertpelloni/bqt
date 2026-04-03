package kernel

import (
	"sync"
	"log"
)

type Clipboard struct {
	mu     sync.RWMutex
	text   string
	binary []byte // Support for images/audio
}

var (
	clipInstance *Clipboard
	clipOnce     sync.Once
)

func GetClipboard() *Clipboard {
	clipOnce.Do(func() { clipInstance = &Clipboard{} })
	return clipInstance
}

func (c *Clipboard) SetText(val string) {
	c.mu.Lock(); defer c.mu.Unlock()
	if c.text != val {
		c.text = val
		log.Printf("OMNI CLIPBOARD: Local String Update.")
	}
}

// SetBinary handles raw byte payloads (Figma-grade assets) in Go.
func (c *Clipboard) SetBinary(data []byte) {
	c.mu.Lock(); defer c.mu.Unlock()
	c.binary = data
	log.Printf("OMNI CLIPBOARD: Local Binary Update (%d bytes).", len(data))
}

func (c *Clipboard) GetBinary() []byte {
	c.mu.RLock(); defer c.mu.RUnlock()
	return c.binary
}

func (c *Clipboard) ReceiveRemote(val string, data []byte) {
	c.mu.Lock(); defer c.mu.Unlock()
	if val != "" { c.text = val }
	if data != nil { c.binary = data }
	log.Println("OMNI CLIPBOARD: Remote Binary Sync Successful.")
}
