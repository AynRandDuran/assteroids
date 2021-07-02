// Some vector math that Raylib didn't give me
//

float v2_magnitude(Vector2 v) {
	return sqrt((v.x*v.x)+(v.y*v.y));
}

Vector2 v2_normal(Vector2 v) {
	float len = v2_magnitude(v);
	Vector2 normal = {v.x/len, v.y/len};
	return normal;
}
