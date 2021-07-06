int enable_shotgun() {
    shotgun_blasts = 6;
    active_powerups |= 1;
    return 1;
}

int disable_shotgun() {
    active_powerups &= 0;
    shotgun_box.w = 0;
    shotgun_blasts = 0;
    return 0;
}
