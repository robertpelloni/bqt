// Type definitions for OmniUI

declare namespace Omni {

    export interface Widget {
        show(): void;
        hide(): void;
        resize(w: number, h: number): void;
    }

    export class Application {
        static instance(): Application;
        quit(): void;
<<<<<<< HEAD

        // Multi-Input methods
        getDevices(): InputDevice[];
        onDeviceConnected: (device: InputDevice) => void;
        onDeviceDisconnected: (device: InputDevice) => void;
    }

    export interface InputDevice {
        id: string;
        name: string;
        type: "mouse" | "keyboard" | "touch" | "gamepad";
        cursorPosition?: { x: number; y: number };
=======
>>>>>>> feature/omni-ui-framework-18001284211800334382
    }

    export class Button implements Widget {
        constructor(label: string);
        show(): void;
        hide(): void;
        resize(w: number, h: number): void;

        onClicked: () => void;
    }

    export class JuceComponent implements Widget {
        constructor();
        show(): void;
        hide(): void;
        resize(w: number, h: number): void;

        loadPreset(path: string): void;
    }
}

declare function print(msg: string): void;
