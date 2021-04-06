vs.1.1

; NekoEngine UI Vertex Shader
;
; Attributes:
; v0	- position and uv
; v1	- color
;
; Constants:
; c0-c3	- MVP matrix

def c4, 0.0, 0.0, 0.0, 1.0

mov r0.xy, v0
mov r0.zw, c4.zwzw
m4x4 oPos, r0, c0

mov oT0.xy, v0.zwzw         
mov oD0, v1
