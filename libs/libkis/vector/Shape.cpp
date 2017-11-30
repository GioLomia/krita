/*
 *  Copyright (c) 2017 Wolthera van Hövell tot Westerflier <griffinvalley@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include "Shape.h"
struct Shape::Private {
    Private() {}
    KoShape *shape;
};

Shape::Shape(KoShape *shape, QObject *parent)
    : QObject(parent)
    , d(new Private)
{
    d->shape = shape;
}

Shape::~Shape()
{
    delete d;
}

QString Shape::name() const
{
    return d->shape->name();
}

void Shape::setName(const QString &name)
{
    d->shape->setName(name);
}

QString Shape::type() const
{
    return d->shape->shapeId();
}

bool Shape::visible()
{
    return d->shape->isVisible();
}

void Shape::setVisible(bool visible)
{
    d->shape->setVisible(visible);
}

KoShape *Shape::shape()
{
    return d->shape;
}
