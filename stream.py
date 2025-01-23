from flask import Flask, Response
import cv2
import threading

# Initialize Flask app
app = Flask(__name__)

# Global variables for video capture
camera = cv2.VideoCapture(0)  # Adjust '0' if you're using an external camera
lock = threading.Lock()

def generate_frames():
    """Stream frames from the camera."""
    while True:
        with lock:  # Ensure thread safety for camera access
            success, frame = camera.read()
            if not success:
                break

            # Encode frame as JPEG
            _, buffer = cv2.imencode('.jpg', frame)
            frame = buffer.tobytes()

        # Yield frame to the client
        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n')

@app.route('/video_feed')
def video_feed():
    """Route to stream the video feed."""
    return Response(generate_frames(), mimetype='multipart/x-mixed-replace; boundary=frame')

@app.route('/')
def index():
    """Landing page with full-screen camera feed."""
    return '''
    <html>
        <head>
            <title>Camera Feed</title>
            <style>
                body, html {
                    margin: 0;
                    padding: 0;
                    height: 100%;
                    display: flex;
                    justify-content: center;
                    align-items: center;
                    background: #000;
                }
                img {
                    width: 100%;
                    height: 100%;
                    object-fit: cover;
                }
            </style>
        </head>
        <body>
            <img src="/video_feed">
        </body>
    </html>
    '''

if __name__ == '__main__':
    try:
        app.run(host='0.0.0.0', port=5000, threaded=True)
    except KeyboardInterrupt:
        print("\nExiting...")
    finally:
        camera.release()
