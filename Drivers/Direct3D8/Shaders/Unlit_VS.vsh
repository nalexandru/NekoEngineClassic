vs.1.1

; NekoEngine Model Vertex Shader
;
; Attributes:
; v0	- position
; v1	- normal
; v2	- tangent
; v3	- uv
;
; Constants:
; c0-c3	- MVP matrix

m4x4 oPos, v0, c0
mov oT0, v3