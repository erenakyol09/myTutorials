Look at only the staged changes (git diff --cached) and write a commit message.

Format:
<type>(<scope>): <short summary>

Types: feat, fix, refactor, docs, test, chore

Rules:
- Summary max 50 characters
- Only describe what is staged, ignore unstaged changes
- If there are no staged changes, warn me instead of writing a message

After writing the message, automatically run:
git commit -m "<the message you wrote>"

Do not ask for confirmation, just commit directly.