.section .rodata
.global _binary_zap_ext_light32_psf_start
.global _binary_zap_ext_light32_psf_end
.global _binary_zap_ext_light32_psf_size

_binary_zap_ext_light32_psf_start:
.incbin "assets/zap-ext-light32.psf"
_binary_zap_ext_light32_psf_end:
_binary_zap_ext_light32_psf_size = _binary_zap_ext_light32_psf_end - _binary_zap_ext_light32_psf_start
