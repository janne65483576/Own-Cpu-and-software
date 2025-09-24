main:
    ADD r0, 10
    ADD r1, 10
    ADD r2, 10
    ADD r3, 10

    ADD r0, r1
    ADD r0, r2
    ADD r0, r3

    SUB r0, &data

data:
    .byte 30