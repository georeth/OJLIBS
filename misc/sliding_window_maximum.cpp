deque<int> window;
for (int i : range(n)) {
    for (; to_push <= i + d * (t - last) && to_push < n; ++to_push) {
	while (!window.empty() && dp[window.back()] <= dp[to_push])
	    window.pop_back();
	window.push_back(to_push);
    }
    while (window.front() < i - d * (t - last))
	window.pop_front();

    cur[i] = dp[window.front()];
}
