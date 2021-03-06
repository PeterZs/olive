/***

  Olive - Non-Linear Video Editor
  Copyright (C) 2019  Olive Team

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

***/

#include "clipboard.h"

#include "timeline/clip.h"
#include "nodes/node.h"
#include "effects/transition.h"
#include "undo/undo.h"

Clipboard::Clipboard() :
  type_(CLIPBOARD_TYPE_CLIP)
{
}

void Clipboard::Append(VoidPtr obj)
{
  clipboard_.append(obj);
}

void Clipboard::Insert(int pos, VoidPtr obj)
{
  clipboard_.insert(pos, obj);
}

void Clipboard::RemoveAt(int pos)
{
  clipboard_.removeAt(pos);
}

void Clipboard::Clear()
{
  clipboard_.clear();
  clipboard_transitions_.clear();
}

int Clipboard::Count()
{
  return clipboard_.size();
}

VoidPtr Clipboard::Get(int i)
{
  return clipboard_.at(i);
}

void Clipboard::SetType(Clipboard::Type type)
{
  if (type == type_) {
    return;
  }

  Clear();
  type_ = type;
}

bool Clipboard::IsEmpty()
{
  return clipboard_.isEmpty();
}

Clipboard::Type Clipboard::type()
{
  return type_;
}

bool Clipboard::DeleteClipsWithMedia(ComboAction *ca, Media *m)
{
  if (type_ != CLIPBOARD_TYPE_CLIP) {
    return false;
  }

  int delete_count = 0;

  for (int i=0;i<clipboard_.size();i++) {
    Clip* c = std::static_pointer_cast<Clip>(clipboard_.at(i)).get();

    if (c->media() == m) {
      ca->append(new RemoveClipsFromClipboard(i-delete_count));
      delete_count++;
    }
  }

  return (delete_count > 0);
}

Clipboard olive::clipboard;
