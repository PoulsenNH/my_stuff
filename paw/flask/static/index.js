function loadPage(url) {
    fetch(url)
        .then(res => {
            if (!res.ok) throw new Error(`HTTP error! ${res.status}`);
            return res.text();
        })
        .then(html => {
            document.getElementById('page-content').innerHTML = html;
        })
        .catch(err => {
            console.error("Failed to load page:", err);
            document.getElementById('page-content').innerHTML = `<p>Error loading page</p>`;
        });
} // Too much error logging

// loads the home page by default
loadPage('/home');

// Adding in the click handlers
document.querySelectorAll('.tab-link').forEach(link => {
    link.addEventListener('click', e => {
        e.preventDefault();
        loadPage(link.getAttribute('href'));
    });
});
