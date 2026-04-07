package kernel

import (
	"sync"
	"log"
)

type Clipboard struct {
	mu   sync.RWMutex
	data map[string][]byte
}

var (
	clipInstance *Clipboard
	clipOnce     sync.Once
)

func GetClipboard() *Clipboard {
	clipOnce.Do(func() {
		clipInstance = &Clipboard{
			data: make(map[string][]byte),
		}
	})
	return clipInstance
}

func (c *Clipboard) SetText(val string) {
	c.mu.Lock()
	defer c.mu.Unlock()
	c.data["text/plain"] = []byte(val)
	log.Printf("OMNI CLIPBOARD: Local String Update.")
}

func (c *Clipboard) GetText() string {
	c.mu.RLock()
	defer c.mu.RUnlock()
	return string(c.data["text/plain"])
}

// SetData handles raw byte payloads with a specific mime type.
func (c *Clipboard) SetData(mime string, data []byte) {
	c.mu.Lock()
	defer c.mu.Unlock()
	c.data[mime] = data
	log.Printf("OMNI CLIPBOARD: Local %s Update (%d bytes).", mime, len(data))
}

func (c *Clipboard) GetData(mime string) []byte {
	c.mu.RLock()
	defer c.mu.RUnlock()
	return c.data[mime]
}

func (c *Clipboard) MimeTypes() []string {
	c.mu.RLock()
	defer c.mu.RUnlock()
	types := make([]string, 0, len(c.data))
	for k := range c.data {
		types = append(types, k)
	}
	return types
}

// SetBinary is kept for backward compatibility but maps to application/octet-stream
func (c *Clipboard) SetBinary(data []byte) {
	c.SetData("application/octet-stream", data)
}

func (c *Clipboard) GetBinary() []byte {
	return c.GetData("application/octet-stream")
}

func (c *Clipboard) ReceiveRemote(val string, data []byte) {
	c.mu.Lock()
	defer c.mu.Unlock()
	if val != "" {
		c.data["text/plain"] = []byte(val)
	}
	if data != nil {
		c.data["application/octet-stream"] = data
	}
	log.Println("OMNI CLIPBOARD: Remote Binary Sync Successful.")
}
