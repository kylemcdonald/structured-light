# Active developers #

  * Mikey Stallard
    * Integrating arcball code into ofxEasyCam
  * Eoghan Cunneen
    * Creating gray decoder project
    * Filling out gray decoder code
  * Kyle McDonald
    * General development
    * Priority queue unwrapping
  * Luke/binarymillenium

# Revisions #

  * Test 2+1 decoder
  * Separate some inheritance
    * DepthExporter accepts PhaseDecoder as argument, PhaseDecoder shouldn't wrap DepthExporter's methods
    * PriorityDecoder and FloodFillDecoder should be PriorityUnwrapper and FloodFillUnwrapper, they extend Unwrapper. ThreePhase and 2+1 use Unwrappers rather than extending them.
    * ThreePhaseDecoder should use a triangulation class rather than inherit DepthDecoder
  * Separate "decode" app into "decode" and "render"
  * Optimize "preview" for higher framerate display

# Removal #

  * RgbThreePhase is probably a dead end

# Additions #

  * Export depth image as scene from renderer (Barry's request)
  * Gray code decoding
  * Priority queue unwrapping
  * Automatic gamma calibration
  * Automatic projecetor/camera extrinsics (geometry) calibration
  * Noise reduction techniques
  * Advanced render engine allowing for shading
  * Explore alternate scanning techniques:
    * two-step (gray+gradient)
    * Natal-style frequency-recovery for absolute depth
    * Frequency modulated scanning as an open alternative to phase modulated scanning

# Completed #

  * Created XCode project (oflab @ v&a)
  * Created preview application (oflab @ v&a)
  * Capture interface developments for 2+1 phase (oflab @ v&a)