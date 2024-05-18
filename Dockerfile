FROM devkitpro/toolchain-base

RUN dkp-pacman -Syyu --noconfirm && \
    dkp-pacman -S --needed --noconfirm gba-dev && \
    dkp-pacman -S --needed --noconfirm armv4t-portlibs && \
    yes | dkp-pacman -Scc

ENV DEVKITARM=${DEVKITPRO}/devkitARM