package data

import (
	"sync"
)

type VFS struct {
	mu     sync.RWMutex
	assets map[string][]byte
}

var (
	vfsInstance *VFS
	vfsOnce     sync.Once
)

func GetVFS() *VFS {
	vfsOnce.Do(func() {
		vfsInstance = &VFS{assets: make(map[string][]byte)}
	})
	return vfsInstance
}

func (v *VFS) Mount(name string, data []byte) {
	v.mu.Lock()
	defer v.mu.Unlock()
	v.assets[name] = data
}

func (v *VFS) GetAsset(name string) []byte {
	v.mu.RLock()
	defer v.mu.RUnlock()
	return v.assets[name]
}
