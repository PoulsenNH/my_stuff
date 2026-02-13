function loadPage(url) {
    fetch(url)
        .then(res => {
            if (!res.ok) throw new Error(`HTTP error! ${res.status}`);
            return res.text();
        })
        .then(html => {
            document.getElementById('page-content').innerHTML = html;
            if (url === '/notes') initNotesPage();
        })
        .catch(err => {
            document.getElementById('page-content').innerHTML = `<p>Error loading page</p>`;
        });
}

loadPage('/home');

document.querySelectorAll('.tab-link').forEach(link => {
    link.addEventListener('click', e => {
        e.preventDefault();
        loadPage(link.getAttribute('href'));
    });
});

function initNotesPage() {
    // Notes form
    const noteForm = document.getElementById('note-form');
    const notesList = document.getElementById('notes-list');

    if (noteForm) {
        noteForm.addEventListener('submit', e => {
            e.preventDefault();
            const name = noteForm.querySelector('input').value;
            const type = noteForm.querySelector('select').value;
            const body = noteForm.querySelector('textarea').value;

            const noteCard = document.createElement('div');
            noteCard.className = 'note-card';

            const noteTitle = document.createElement('h4');
            noteTitle.textContent = `${name} (${type})`;

            const noteText = document.createElement('p');
            noteText.textContent = body.length > 20 ? body.slice(0,20) + '...' : body;

            noteCard.addEventListener('click', () => {
                noteText.textContent = noteText.textContent.length > 23 ? body : body.slice(0,20) + '...';
            });

            noteCard.appendChild(noteTitle);
            noteCard.appendChild(noteText);
            notesList.prepend(noteCard);

            noteForm.reset();
        });
    }

    // Inject modal HTML if not present
    if (!document.getElementById('create-room-modal')) {
        const modalHTML = `
        <div id="create-room-modal" class="modal">
            <div id="create-room-content">
                <h3>Create Room</h3>
                <form id="create-room-form">
                    <label for="room-name">Room Name:</label>
                    <input type="text" id="room-name" name="room-name" placeholder="Enter room name">
                    <label for="room-type">Type:</label>
                    <select id="room-type" name="room-type">
                        <option value="public">Public</option>
                        <option value="private">Private</option>
                    </select>
                    <div id="create-room-buttons">
                        <button type="submit" class="create-room-btn">Create</button>
                        <button type="button" class="close-room-btn">Cancel</button>
                    </div>
                </form>
            </div>
        </div>`;
        document.body.insertAdjacentHTML('beforeend', modalHTML);
    }

    // Attach modal event listeners AFTER injecting the modal
    const modal = document.getElementById('create-room-modal');
    const openBtn = document.getElementById('create-room-btn');
    const closeBtn = modal.querySelector('.close-room-btn');
    const createForm = document.getElementById('create-room-form');

    if (openBtn && closeBtn && createForm) {
        openBtn.addEventListener('click', () => modal.classList.add('show'));
        closeBtn.addEventListener('click', () => modal.classList.remove('show'));
        createForm.addEventListener('submit', e => {
            e.preventDefault();
            const roomName = document.getElementById('room-name').value;
            const roomType = document.getElementById('room-type').value;
            console.log(`Room created: ${roomName} (${roomType})`);
            modal.classList.remove('show');
            createForm.reset();
        });
    }
}
